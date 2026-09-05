#include "Player.h"
#include "Engine.h"
#include "Projectile.h"
#include "Random.h"
#include "glm/trigonometric.hpp"
#include <SDL3/SDL_mouse.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

Player::Player(const std::string& texturePath)
{
    mTexture = Engine::instance->textureManager->Get(texturePath);
}

void Player::Update(float dt)
{
    const auto& input = Engine::instance->input;

    glm::vec2 vel = glm::vec2(0.0f);
    if (input->IsDown(SDLK_W))
    {
        vel.y += 1.0f;
    }
    if (input->IsDown(SDLK_A))
    {
        vel.x += -1.0f;
    }
    if (input->IsDown(SDLK_S))
    {
        vel.y += -1.0f;
    }
    if (input->IsDown(SDLK_D))
    {
        vel.x += 1.0f;
    }

    if (glm::length2(vel) > 0.0f)
        vel = glm::normalize(vel);

    auto oldPosition = position;
    position += vel * mSpeed * dt;

    if (position != oldPosition)
    {
        std::printf("new pos: %.0f,%.0f\n", position.x, position.y);
    }

    // Shooting
    if (input->MousePressed(SDL_BUTTON_LEFT))
    {
        glm::vec2 mouseScreenPos(input->MouseX(), input->MouseY());
        glm::vec2 mouseWorldPos = Engine::instance->camera.ScreenToWorld(mouseScreenPos);

        glm::vec2 dir = mouseWorldPos - position;

        if (glm::length2(dir) > 0.0f)
        {
            dir = glm::normalize(dir);
        }

        const int pelletCount = 6;
        const float spreadDegrees = 15.0f;
        const float bulletSpeed = 300.0f;
        const float speedVariance = 0.05f;

        float baseAngle = std::atan2(dir.y, dir.x);

        float spreadRadians = glm::radians(spreadDegrees);

        for (int i = 0; i < pelletCount; ++i)
        {
            float angle = baseAngle + Random::Float(-spreadRadians * 0.5f, spreadRadians * 0.5f);

            glm::vec2 pelletDir(std::cos(angle), std::sin(angle));

            float speedMultiplier = Random::Float(1.0f - speedVariance, 1.0f + speedVariance);
            float pelletSpeed = bulletSpeed * speedMultiplier;

            auto* projectile = Engine::instance->world->CreateEntity<Projectile>("data/bullet.png");

            projectile->position = position;
            projectile->rotation = glm::degrees(angle);

            glm::vec2 absoluteBulletVelocity = (pelletDir * pelletSpeed) + vel;

            projectile->SetVelocity(absoluteBulletVelocity);
        }
    }
}
void Player::Render()
{
    Engine::instance->renderer->DrawSprite(mTexture, position, glm::vec2(13, 19) * scale, rotation,
                                           color);
}
