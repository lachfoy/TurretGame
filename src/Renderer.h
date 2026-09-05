#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

struct RendererImpl;
class Camera;

class Renderer
{
  public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool Init();
    void Shutdown();

    void Clear(float r, float g, float b);

    void BeginFrame(const Camera& camera);
    void EndFrame();

    Texture LoadTexture(const char* path);
    void DeleteTexture(Texture texture);

    void DrawSprite(Texture texture, glm::vec2 position, glm::vec2 size,
                    float rotationDegrees = 0.0f, glm::vec4 color = glm::vec4(1.0f),
                    glm::vec4 uvRect = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

  private:
    RendererImpl* mImpl = nullptr;
};