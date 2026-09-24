#pragma once

#include "GameObject.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class Projectile : public GameObject
{
  public:
    explicit Projectile(const std::string& texturePath);

    void Update(float dt) override;
    void Render() override;

    void SetVelocity(glm::vec2 velocity) { m_velocity = velocity; }

  private:
    Texture m_texture;
    glm::vec4 m_color = glm::vec4(1.0f);
    glm::vec2 m_velocity = glm::vec2(0.0f);
};
