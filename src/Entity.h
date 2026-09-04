#pragma once

#include <glm/glm.hpp>

class World;

class Entity
{
  public:
    virtual ~Entity() = default;

    virtual void Update(float /*dt*/) {}
    virtual void Render() {}

    void Destroy() { mPendingDestroy = true; }
    bool IsPendingDestroy() const { return mPendingDestroy; }

    glm::vec2 position = glm::vec2(0, 0);
    float rotation = 0.0f;
    glm::vec2 scale = glm::vec2(1, 1);

  protected:
    World* mWorld = nullptr;

  private:
    bool mPendingDestroy = false;
    friend class World;
};
