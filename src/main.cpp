
#include "Engine.h"
#include "Player.h"

class DemoWorld : public World
{
  public:
    void Init() override
    {
        World::Init();
        Engine::instance->textureManager->Load("data/guy.png");
        Engine::instance->textureManager->Load("data/bullet.png");

        mPlayer = CreateEntity<Player>("data/guy.png");
    }

    void Update(float dt) override
    {
        World::Update(dt);
        Engine::instance->camera.SetPosition(mPlayer->position);
    }

    void Shutdown() override
    {
        World::Shutdown();
        Engine::instance->textureManager->Unload("data/bullet.png");
        Engine::instance->textureManager->Unload("data/guy.png");
    }

  private:
    Player* mPlayer;
};

int main()
{
    Engine engine;

    if (!engine.Init())
        return 1;

    engine.SetWorld(std::make_unique<DemoWorld>());

    engine.Run();

    return 0;
}
