#pragma once

#include "Entity.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class Projectile : public Entity
{
  public:
    explicit Projectile(const std::string& texturePath);

    void Update(float dt) override;
    void Render() override;

    void SetVelocity(glm::vec2 velocity) { mVelocity = velocity; }

  private:
    Texture mTexture;
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec2 mVelocity = glm::vec2(0.0f);
};
