#pragma once

#include <glm/glm.hpp>

class GameObject
{
  public:
    virtual ~GameObject() = default;

    virtual void Update(float /*dt*/) {}
    virtual void Render() {}

    void Destroy() { m_pendingDestroy = true; }
    bool IsPendingDestroy() const { return m_pendingDestroy; }

    glm::vec2 position = glm::vec2(0, 0);
    float rotation = 0.0f;
    glm::vec2 scale = glm::vec2(1, 1);

  private:
    bool m_pendingDestroy = false;
};
