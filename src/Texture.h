#pragma once

#include <cstdint>

using TextureId = std::uintptr_t;

static constexpr TextureId kInvalidTextureId = 0;

struct Texture
{
    TextureId id = kInvalidTextureId;
    int width = -1;
    int height = -1;
};