#include "Projectile.h"
#include "Engine.h"

Projectile::Projectile(const std::string& texturePath)
{
    m_texture = Engine::instance->textureManager->Get(texturePath);
}

void Projectile::Update(float dt) { position += m_velocity * dt; }

void Projectile::Render()
{
    Engine::instance->renderer->DrawSprite(m_texture, position, glm::vec2(4, 4) * scale, rotation,
                                           m_color);
}
