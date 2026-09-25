#include "Enemy.h"
#include "Engine.h"
#include "Player.h"

Enemy::Enemy(const std::string& texturePath)
{
    m_texture = Engine::instance->textureManager->Get(texturePath);
}

void Enemy::Update(float)
{
    // Get the first player
    Player* target = nullptr;
    for (const auto& player : Engine::instance->world->GetGameObjects<Player>())
    {
        target = player;
        break;
    }

    if (target)
    {
    }
}

void Enemy::Render() {}
