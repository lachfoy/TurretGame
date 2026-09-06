#include "Game.h"
#include "Engine.h"
#include "Player.h"

void Game::Init()
{
    Engine::instance->textureManager->Load("data/guy.png");
    Engine::instance->textureManager->Load("data/bullet.png");

    mPlayer = Engine::instance->world->CreateGameObject<Player>("data/guy.png");
}

void Game::Shutdown()
{
    Engine::instance->textureManager->Unload("data/bullet.png");
    Engine::instance->textureManager->Unload("data/guy.png");
}

void Game::Update(float /*dt*/) { Engine::instance->camera.SetPosition(mPlayer->position); }
