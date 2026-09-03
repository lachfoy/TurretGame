#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class World;

class Entity
{
  public:
    friend class World;

    virtual ~Entity() = default;

    virtual void Update([[maybe_unused]] float dt) {}
    virtual void Render() {}

    void Destroy() { mPendingDestroy = true; }
    bool IsPendingDestroy() const { return mPendingDestroy; }

    glm::mat4 GetTransform() const
    {
        glm::mat4 t = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f));
        t = glm::rotate(t, glm::radians(rotation), glm::vec3(0, 0, 1));
        t = glm::scale(t, glm::vec3(scale, 1.0f));
        return t;
    }

    glm::vec2 position = glm::vec2(0, 0);
    float rotation = 0.0f; // degrees, around Z (2D rotation)
    glm::vec2 scale = glm::vec2(1, 1);

  protected:
    World* mWorld = nullptr;

  private:
    bool mPendingDestroy = false;
};
