#include "Player.h"
#include "Engine.h"
#include "Projectile.h"
#include "glm/trigonometric.hpp"
#include <SDL3/SDL_mouse.h>
#include <glm/gtc/random.hpp>

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

    if (glm::length(vel) > 0.0f)
        vel = glm::normalize(vel);

    auto oldPosition = position;
    position += vel * mSpeed * dt;

    if (position != oldPosition)
    {
        std::printf("new pos: %.0f,%.0f\n", position.x, position.y);
    }

    // Shooting
    mShotCooldown -= dt;
    if (mShotCooldown < 0.0f)
    {
        mShotCooldown = 0.0f;
    }

    if (input->MousePressed(SDL_BUTTON_LEFT) && mShotCooldown <= 0.0f)
    {
        glm::vec2 mouseScreenPos(input->MouseX(), input->MouseY());
        glm::vec2 mouseWorldPos = Engine::instance->camera.ScreenToWorld(mouseScreenPos);

        glm::vec2 dir = mouseWorldPos - position;

        if (glm::length(dir) > 0.0f)
        {
            dir = glm::normalize(dir);
        }

        const int pelletCount = 6;
        const float spreadDegrees = 15.0f;
        const float bulletSpeed = 300.0f;
        const float speedVariance = 0.05f;

        float baseAngle = atan2(dir.y, dir.x);

        float spreadRadians = glm::radians(spreadDegrees);

        for (int i = 0; i < pelletCount; ++i)
        {
            float angleOffset = glm::linearRand(-spreadRadians * 0.5f, spreadRadians * 0.5f);

            float angle = baseAngle + angleOffset;

            glm::vec2 pelletDir(cos(angle), sin(angle));

            float random = static_cast<float>(rand()) / RAND_MAX;
            float speedMultiplier = 1.0f + (random - 0.5f) * 2.0f * speedVariance;

            float pelletSpeed = bulletSpeed * speedMultiplier;

            auto* projectile = Engine::instance->world->CreateEntity<Projectile>("data/bullet.png");

            projectile->position = position;
            projectile->rotation = glm::degrees(angle);

            glm::vec2 absoluteBulletVelocity = (pelletDir * pelletSpeed) + vel;

            projectile->SetVelocity(absoluteBulletVelocity);
        }

        mShotCooldown = kShotCooldown;
    }
}
void Player::Render()
{
    Engine::instance->renderer->DrawSprite(mTexture, position, glm::vec2(13, 19) * scale, rotation,
                                           color);
}
