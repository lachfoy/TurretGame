#include "Renderer.h"
#include "Camera.h"
#include "Texture.h"

#include <glad/glad.h>

#pragma warning(push)
#pragma warning(disable : 26451)
#pragma warning(disable : 26819)
#pragma warning(disable : 6262)
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#pragma warning(pop)

#include <cassert>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace
{
constexpr const char* kVertexSrc = R"(
    #version 460 core
    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec2 aUv;
    layout (location = 2) in vec4 aColor;

    uniform mat4 uViewProj;

    out vec2 vUv;
    out vec4 vColor;

    void main()
    {
        vUv = aUv;
        vColor = aColor;
        gl_Position = uViewProj * vec4(aPos, 0.0, 1.0);
    }
)";

constexpr const char* kFragmentSrc = R"(
    #version 460 core
    in vec2 vUv;
    in vec4 vColor;

    uniform sampler2D uTexture;

    out vec4 FragColor;

    void main()
    {
        vec4 texColor = texture(uTexture, vUv);
        if (texColor.a < 0.01)
            discard;
        FragColor = texColor * vColor;
    }
)";

GLuint CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Shader compile error: %s\n", log);
    }

    return shader;
}

GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), nullptr, log);
        std::fprintf(stderr, "Shader link error: %s\n", log);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}
} // namespace

struct GLTexture
{
    GLuint id;
    int width;
    int height;
};

struct Vertex
{
    glm::vec2 position;
    glm::vec2 uv;
    glm::vec4 color;
};

struct RendererImpl
{
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ibo = 0;
    GLuint shader = 0;
    GLuint whiteTexture = 0;

    GLuint currentTexture = 0;
    std::vector<Vertex> vertices;
    size_t maxQuadsPerFlush = 2000;

    glm::mat4 viewProj{1.0f};
};

namespace
{
void Flush(RendererImpl& impl)
{
    if (impl.vertices.empty())
        return;

    glUseProgram(impl.shader);
    glUniformMatrix4fv(glGetUniformLocation(impl.shader, "uViewProj"), 1, GL_FALSE,
                       glm::value_ptr(impl.viewProj));
    glUniform1i(glGetUniformLocation(impl.shader, "uTexture"), 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, impl.currentTexture);

    glBindVertexArray(impl.vao);
    glBindBuffer(GL_ARRAY_BUFFER, impl.vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, impl.vertices.size() * sizeof(Vertex),
                    impl.vertices.data());

    size_t quadCount = impl.vertices.size() / 4;
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(quadCount * 6), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    impl.vertices.clear();
}
} // namespace

Renderer::Renderer() { mImpl = new RendererImpl(); }

Renderer::~Renderer() { delete mImpl; }

bool Renderer::Init()
{
    mImpl->vertices.reserve(mImpl->maxQuadsPerFlush * 4);

    GLuint vs = CompileShader(GL_VERTEX_SHADER, kVertexSrc);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, kFragmentSrc);
    mImpl->shader = LinkProgram(vs, fs);

    glGenVertexArrays(1, &mImpl->vao);
    glGenBuffers(1, &mImpl->vbo);
    glGenBuffers(1, &mImpl->ibo);

    glBindVertexArray(mImpl->vao);

    // Vertex buffer is re-uploaded every flush, so just reserve worst-case size up front.
    glBindBuffer(GL_ARRAY_BUFFER, mImpl->vbo);
    glBufferData(GL_ARRAY_BUFFER, mImpl->maxQuadsPerFlush * 4 * sizeof(Vertex), nullptr,
                 GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    // Index pattern (0,1,2, 2,3,0) repeats per quad - generate it once up front.
    std::vector<uint32_t> indices(mImpl->maxQuadsPerFlush * 6);
    for (size_t i = 0; i < mImpl->maxQuadsPerFlush; ++i)
    {
        uint32_t base = static_cast<uint32_t>(i * 4);
        size_t o = i * 6;
        indices[o + 0] = base + 0;
        indices[o + 1] = base + 1;
        indices[o + 2] = base + 2;
        indices[o + 3] = base + 2;
        indices[o + 4] = base + 3;
        indices[o + 5] = base + 0;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mImpl->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(),
                 GL_STATIC_DRAW);

    glBindVertexArray(0);

    // 1x1 white pixel - used whenever DrawSprite() gets no texture, so solid-color
    // rects work without every caller needing a dummy texture.
    glGenTextures(1, &mImpl->whiteTexture);
    glBindTexture(GL_TEXTURE_2D, mImpl->whiteTexture);
    unsigned char whitePixel[] = {0xff, 0xff, 0xff, 0xff};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Renderer::Shutdown()
{
    if (mImpl->whiteTexture)
        glDeleteTextures(1, &mImpl->whiteTexture);
    if (mImpl->ibo)
        glDeleteBuffers(1, &mImpl->ibo);
    if (mImpl->vbo)
        glDeleteBuffers(1, &mImpl->vbo);
    if (mImpl->vao)
        glDeleteVertexArrays(1, &mImpl->vao);
    if (mImpl->shader)
        glDeleteProgram(mImpl->shader);

    mImpl->whiteTexture = mImpl->ibo = mImpl->vbo = mImpl->vao = mImpl->shader = 0;
}

void Renderer::Clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::BeginFrame(const Camera& camera)
{
    int vpX, vpY, vpWidth, vpHeight;
    camera.GetViewportRect(vpX, vpY, vpWidth, vpHeight);
    glViewport(vpX, vpY, vpWidth, vpHeight);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);

    mImpl->viewProj = camera.GetViewProj();
    mImpl->vertices.clear();
    mImpl->currentTexture = 0;
}

void Renderer::EndFrame() { Flush(*mImpl); }

Texture Renderer::LoadTexture(const char* path)
{
    int w, h, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &w, &h, &channels, 4);

    if (!data)
    {
        std::fprintf(stderr, "Failed to load texture: %s\n", path);
        return {};
    }

    std::printf("Loaded texture: %s\n", path);

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLTexture* tex = new GLTexture();
    tex->id = id;
    tex->width = w;
    tex->height = h;

    return {reinterpret_cast<uintptr_t>(tex), w, h};
}

void Renderer::DeleteTexture(Texture texture)
{
    assert(texture.id != kInvalidTextureId);

    GLTexture* tex = reinterpret_cast<GLTexture*>(texture.id);
    glDeleteTextures(1, &tex->id);
    delete tex;
}

void Renderer::DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size,
                          float rotationDegrees, glm::vec4 color, glm::vec4 uvRect)
{
    GLuint glId = texture.id != kInvalidTextureId ? reinterpret_cast<GLTexture*>(texture.id)->id
                                                  : mImpl->whiteTexture;

    // flush whatevers queued under the old texture first.
    if (glId != mImpl->currentTexture && mImpl->currentTexture != 0)
        Flush(*mImpl);

    mImpl->currentTexture = glId;

    if (mImpl->vertices.size() / 4 >= mImpl->maxQuadsPerFlush)
        Flush(*mImpl);

    glm::vec2 half = size * 0.5f;
    float rad = glm::radians(rotationDegrees);
    float c = std::cos(rad);
    float s = std::sin(rad);

    glm::vec2 corners[4] = {
        {-half.x, -half.y},
        {half.x, -half.y},
        {half.x, half.y},
        {-half.x, half.y},
    };

    glm::vec2 uvs[4] = {
        {uvRect.x, uvRect.y},
        {uvRect.z, uvRect.y},
        {uvRect.z, uvRect.w},
        {uvRect.x, uvRect.w},
    };

    for (int i = 0; i < 4; ++i)
    {
        glm::vec2 rotated(corners[i].x * c - corners[i].y * s, corners[i].x * s + corners[i].y * c);
        mImpl->vertices.push_back({position + rotated, uvs[i], color});
    }
}