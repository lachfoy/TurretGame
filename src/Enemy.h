#pragma once

#include "GameObject.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <string>

class Enemy : public GameObject
{
  public:
    explicit Enemy(const std::string& texturePath);

    void Update(float dt) override;
    void Render() override;

  private:
    Texture m_texture;
    glm::vec4 m_color = glm::vec4(1.0f);
    GameObject* m_target = nullptr;
};
