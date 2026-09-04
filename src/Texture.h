#pragma once

#include <cstdint>

static constexpr std::uintptr_t kInvalidTextureId = 0;

struct Texture
{
    std::uintptr_t id = kInvalidTextureId;
    int width = -1;
    int height = -1;
};