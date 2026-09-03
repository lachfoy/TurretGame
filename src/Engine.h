#pragma once

#include <SDL3/SDL.h>
#include <memory>

#include "Input.h"
#include "Renderer.h"
#include "World.h"

class Engine
{
  public:
    static constexpr int kDefaultWindowWidth = 1280;
    static constexpr int kDefaultWindowHeight = 720;

    Engine() { instance = this; }
    ~Engine();

    bool Init();
    void Shutdown();
    void Run();

    void SetWorld(std::unique_ptr<World> world);

    SDL_Window* window = nullptr;
    SDL_GLContext glContext = nullptr;

    std::unique_ptr<Input> input;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<World> world;

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
