#pragma once

class Player;

class Game
{
  public:
    void Init();
    void Shutdown();

    void Update(float dt);

  private:
    Player* m_player = nullptr;
};