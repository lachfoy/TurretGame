#pragma once

#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <memory>
#include <vector>

#include "Entity.h"

class Player;

class World
{
  public:
    World() {}
    ~World() {}

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    void Init();

    void Shutdown();

    template <typename T, typename... Args> T* CreateEntity(Args&&... args)
    {
        static_assert(std::is_base_of_v<Entity, T>);
        auto e = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = e.get();
        mPendingEntities.push_back(std::move(e));
        return ptr;
    }

    template <typename T, typename Predicate> std::vector<T*> Query(Predicate&& pred) const
    {
        std::vector<T*> result;
        for (auto& e : mEntities)
        {
            if (T* t = dynamic_cast<T*>(e.get()))
                if (pred(*t))
                    result.push_back(t);
        }
        return result;
    }

    template <typename T> std::vector<T*> Query() const
    {
        return Query<T>([](const T&) { return true; });
    }

    template <typename T>
    std::vector<T*> QueryInRadius(const World& world, glm::vec2 center, float radius)
    {
        float r2 = radius * radius;
        return world.Query<T>([&](const T& t) { return glm::length2(t.position - center) <= r2; });
    }

    void Update(float dt);
    void Render();

  private:
    std::vector<std::unique_ptr<Entity>> mEntities;
    std::vector<std::unique_ptr<Entity>> mPendingEntities;
    Player* mPlayer;
};
