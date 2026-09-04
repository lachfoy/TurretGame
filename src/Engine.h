#pragma once

#include <SDL3/SDL.h>
#include <memory>

#include "Camera.h"
#include "Input.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "World.h"

class Engine
{
  public:
    static constexpr int kDefaultWindowWidth = 960;
    static constexpr int kDefaultWindowHeight = 540;

    Engine() { instance = this; }
    ~Engine();

    bool Init();
    void Shutdown();
    void Run();

    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;

    std::unique_ptr<Input> input;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<World> world;
    Camera camera;
    std::unique_ptr<TextureManager> textureManager;

    double fixedDelta = 1.0 / 60.0;
    double accumulator = 0.0;

    static Engine* instance;

  private:
    bool InitSdl();
    bool InitWindowAndGlContext();
    bool InitGl();
    void InitSubsystems();

    bool mShutdown = false;
};
