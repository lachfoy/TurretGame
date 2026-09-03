#pragma once

#include <cstdint>
#include <glm/glm.hpp>

// A texture handle. `id` is an opaque pointer to a GLTexture, heap-allocated by
// LoadTexture()/CreateTexture() - don't interpret it as anything beyond "pass it
// back to Renderer", and don't forget to DeleteTexture() it.
struct Texture
{
    uintptr_t id = 0;
    int width = 0;
    int height = 0;
};

struct RendererImpl;

// The entire 2D rendering API. Internally this batches every DrawSprite() call
// into one growing vertex buffer and only actually issues a draw call when
// something that would break the batch changes (currently: the texture) or
// when EndFrame() is called. Callers never see a GL handle, shader, or draw
// call - just Init/BeginFrame/DrawSprite/EndFrame. All GL state lives in
// RendererImpl, defined in Renderer.cpp.
class Renderer
{
  public:
    Renderer();
    ~Renderer();

    bool Init();
    void Shutdown();

    void Clear(float r, float g, float b);

    // windowWidth/windowHeight in pixels. cameraPos/zoom place the 2D camera in
    // world space - position is the world point at the screen center.
    void BeginFrame(int windowWidth, int windowHeight, glm::vec2 cameraPos = glm::vec2(0.0f),
                    float zoom = 1.0f);
    void EndFrame();

    Texture LoadTexture(const char* path);
    void DeleteTexture(Texture texture);

    // Draws one quad. position/size are in world units, rotationDegrees rotates
    // around the quad's center. uvRect is (u0, v0, u1, v1) into the texture -
    // leave it default to use the whole texture. Pass Texture{} (id 0) to draw
    // a plain colored rect using the renderer's built-in white pixel.
    //
    // May flush the current batch (one draw call) if this draw uses a
    // different texture than the last one, or if the batch is full.
    void DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size,
                    float rotationDegrees = 0.0f, glm::vec4 color = glm::vec4(1.0f),
                    glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

  private:
    RendererImpl* mImpl = nullptr;
};