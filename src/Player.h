#pragma once

#include "GameObject.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class Player : public GameObject
{
  public:
    explicit Player(const std::string& texturePath);

    void Update(float dt) override;
    void Render() override;

  private:
    Texture mTexture;
    glm::vec4 mColor = glm::vec4(1.0f);
    float mSpeed = 50.0f;
};
