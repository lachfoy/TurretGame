
#include "World.h"
#include "Engine.h"
#include "Player.h"

void World::Init()
{
    Engine::instance->textureManager->Load("data/guy.png");
    Engine::instance->textureManager->Load("data/bullet.png");

    mPlayer = CreateEntity<Player>("data/guy.png");
}

void World::Shutdown()
{
    Engine::instance->textureManager->Unload("data/bullet.png");
    Engine::instance->textureManager->Unload("data/guy.png");
}

void World::Update(float dt)
{
    for (const auto& e : mEntities)
    {
        e->Update(dt);
    }

    for (auto& e : mPendingEntities)
    {
        mEntities.push_back(std::move(e));
    }
    mPendingEntities.clear();

    mEntities.erase(std::remove_if(mEntities.begin(), mEntities.end(),
                                   [](const auto& e) { return e->IsPendingDestroy(); }),
                    mEntities.end());

    Engine::instance->camera.SetPosition(mPlayer->position);
}

void World::Render()
{
    for (auto& e : mEntities)
        e->Render();
}
