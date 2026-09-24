#include "TextureManager.h"
#include "Engine.h"
#include <cassert>

void TextureManager::Load(const std::string& path)
{
    assert(m_textures.find(path) == m_textures.end());
    m_textures[path] = Engine::instance->renderer->LoadTexture(path.c_str());
}

void TextureManager::Unload(const std::string& path)
{
    auto it = m_textures.find(path);
    assert(it != m_textures.end());

    std::printf("Unloading texture: %s\n", it->first.c_str());
    Engine::instance->renderer->DeleteTexture(it->second);
    m_textures.erase(it);
}

void TextureManager::UnloadAll()
{
    for (const auto& pair : m_textures)
    {
        std::printf("Unloading texture: %s\n", pair.first.c_str());
        Engine::instance->renderer->DeleteTexture(pair.second);
    }
    m_textures.clear();
}

Texture TextureManager::Get(const std::string& path) const
{
    auto it = m_textures.find(path);
    assert(it != m_textures.end());

    return it->second;
}
