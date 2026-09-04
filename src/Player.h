#pragma once

#include "Entity.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class Player : public Entity
{
  public:
    explicit Player(const std::string& texturePath);

    void Update(float dt) override;
    void Render() override;

  private:
    Texture mTexture;
    glm::vec4 color = glm::vec4(1.0f);
    float mSpeed = 50.0f;
    float mShotCooldown = 0.0f;
    static constexpr float kShotCooldown = 0.01f;
};
