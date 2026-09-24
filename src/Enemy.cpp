#include "Enemy.h"
#include "Engine.h"

Enemy::Enemy(const std::string& texturePath)
{
    m_texture = Engine::instance->textureManager->Get(texturePath);
}

void Enemy::Update(float) {}

void Enemy::Render() {}
