#include "TextureManager.h"
#include "Engine.h"
#include <cassert>

void TextureManager::Load(const std::string& path)
{
    assert(mTextures.find(path) == mTextures.end());
    mTextures[path] = Engine::instance->renderer->LoadTexture(path.c_str());
}

void TextureManager::Unload(const std::string& path)
{
    auto it = mTextures.find(path);
    assert(it != mTextures.end());

    std::printf("Unloading texture: %s\n", it->first.c_str());
    Engine::instance->renderer->DeleteTexture(it->second);
    mTextures.erase(it);
}

void TextureManager::UnloadAll()
{
    for (const auto& pair : mTextures)
    {
        std::printf("Unloading texture: %s\n", pair.first.c_str());
        Engine::instance->renderer->DeleteTexture(pair.second);
    }
    mTextures.clear();
}

Texture TextureManager::Get(const std::string& path) const
{
    auto it = mTextures.find(path);
    assert(it != mTextures.end());

    return it->second;
}
