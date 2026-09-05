#pragma once

#include "Texture.h"
#include <string>
#include <unordered_map>

class TextureManager
{
  public:
    TextureManager() {}
    ~TextureManager() { UnloadAll(); }

    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;

    void Load(const std::string& path);
    void Unload(const std::string& path);
    void UnloadAll();
    Texture Get(const std::string& path) const;

  private:
    std::unordered_map<std::string, Texture> mTextures;
};