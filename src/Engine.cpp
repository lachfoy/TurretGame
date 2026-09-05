#include "Engine.h"
#include "Random.h"
#include "TextureManager.h"

#include <glad/glad.h>

#include <algorithm>
#include <cstdio>
#include <memory>

Engine* Engine::instance = nullptr;

Engine::~Engine() { Shutdown(); }

bool Engine::InitSdl()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
    {
        std::fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }

    const int v = SDL_GetVersion();
    std::printf("Using SDL version %d.%d.%d\n", SDL_VERSIONNUM_MAJOR(v), SDL_VERSIONNUM_MINOR(v),
                SDL_VERSIONNUM_MICRO(v));

    return true;
}

bool Engine::InitWindowAndGlContext()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("TurretGame", kDefaultWindowWidth, kDefaultWindowHeight,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }

    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::fprintf(stderr, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

bool Engine::InitGl()
{
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::fprintf(stderr, "Failed to initialize GLAD\n");
        return false;
    }

    std::printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
    return true;
}

void Engine::InitSubsystems()
{
    input = std::make_unique<Input>();

    renderer = std::make_unique<Renderer>();
    renderer->Init();

    textureManager = std::make_unique<TextureManager>();

    Random::Init(12345);

    world = std::make_unique<World>();
    world->Init();
}

bool Engine::Init()
{
    if (!InitSdl())
        return false;

    if (!InitWindowAndGlContext())
        return false;

    if (!InitGl())
        return false;

    InitSubsystems();

    return true;
}

void Engine::Shutdown()
{
    if (mShutdown)
        return;
    mShutdown = true;

    if (world)
    {
        world->Shutdown();
        world.reset();
    }

    if (renderer)
    {
        renderer->Shutdown();
        renderer.reset();
    }

    input.reset();

    textureManager.reset();

    if (glContext)
    {
        SDL_GL_DestroyContext(glContext);
        glContext = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();

    if (instance == this)
        instance = nullptr;
}

void Engine::Run()
{
    Uint64 lastTicks = SDL_GetTicksNS();
    bool running = true;

    while (running)
    {
        Uint64 nowTicks = SDL_GetTicksNS();
        double frameTime = (nowTicks - lastTicks) / 1e9;
        lastTicks = nowTicks;

        frameTime = std::min(frameTime, 0.25);

        input->BeginFrame();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            input->HandleEvent(event);

            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        accumulator += frameTime;
        while (accumulator >= fixedDelta)
        {
            if (world)
                world->Update((float)fixedDelta);

            accumulator -= fixedDelta;
        }

        renderer->Clear(0.05f, 0.05f, 0.08f);

        int windowWidth, windowHeight;
        SDL_GetWindowSizeInPixels(window, &windowWidth, &windowHeight);
        camera.SetViewport(windowWidth, windowHeight);

        renderer->BeginFrame(camera);

        if (world)
            world->Render();

        renderer->EndFrame();

        SDL_GL_SwapWindow(window);
    }
}
