#include "Projectile.h"
#include "Engine.h"

Projectile::Projectile(const std::string& texturePath)
{
    mTexture = Engine::instance->textureManager->Get(texturePath);
}

void Projectile::Update(float dt) { position += mVelocity * dt; }

void Projectile::Render()
{
    Engine::instance->renderer->DrawSprite(mTexture, position, glm::vec2(4, 4) * scale, rotation,
                                           color);
}
