
#include "Engine.h"

class Player : public Entity
{
  public:
    explicit Player(Texture texture) : mTexture(texture) {}

    void Update(float dt) override
    {
        const auto& input = Engine::instance->input;

        glm::vec2 vel = glm::vec2(0.0f);
        if (input->IsDown(SDLK_W))
        {
            vel.y += 1.0f;
        }
        if (input->IsDown(SDLK_A))
        {
            vel.x += -1.0f;
        }
        if (input->IsDown(SDLK_S))
        {
            vel.y += -1.0f;
        }
        if (input->IsDown(SDLK_D))
        {
            vel.x += 1.0f;
        }

        if (glm::length(vel) > 0.0f)
            vel = glm::normalize(vel);

        auto oldPosition = position;
        position += vel * mSpeed * dt;

        if (position != oldPosition)
        {
            std::printf("new pos: %.0f,%.0f\n", position.x, position.y);
        }
    }

    void Render() override
    {
        Engine::instance->renderer->DrawSprite(mTexture, position, glm::vec2(13, 19) * scale,
                                               rotation, color);
    }

  private:
    Texture mTexture;
    glm::vec4 color = glm::vec4(1.0f);
    float mSpeed = 200.0f;
};

class DemoWorld : public World
{
  public:
    void Init() override
    {
        mPlayerTexture = Engine::instance->renderer->LoadTexture("data/guy.png");
        mPlayer = CreateEntity<Player>(mPlayerTexture);
        // mPlayer->position = glm::vec2(300, 300);
    }

    void Shutdown() override { Engine::instance->renderer->DeleteTexture(mPlayerTexture); }

  private:
    Player* mPlayer;
    Texture mPlayerTexture;
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
