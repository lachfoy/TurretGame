#pragma once

#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <memory>
#include <vector>

#include "GameObject.h"

class World
{
  public:
    World() {}
    ~World() {}

    World(const World&) = delete;
    World& operator=(const World&) = delete;

    template <typename T, typename... Args> T* CreateGameObject(Args&&... args)
    {
        static_assert(std::is_base_of_v<GameObject, T>);
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* ptr = obj.get();
        mPendingAdds.push_back(std::move(obj));
        return ptr;
    }

    void Update(float dt);
    void Render();

    template <typename T> std::vector<T*> GetGameObjects() const
    {
        std::vector<T*> res;
        for (auto& obj : mGameObjects)
        {
            if (T* t = dynamic_cast<T*>(obj.get()))
            {
                res.push_back(t);
            }
        }
        return res;
    }

    template <typename T> std::vector<T*> GetGameObjectsInRadius(glm::vec2 center, float radius)
    {
        std::vector<T*> res;
        float r2 = radius * radius;
        for (auto& obj : mGameObjects)
        {
            if (T* t = dynamic_cast<T*>(obj.get()))
            {
                if (glm::length2(t->position - center) <= r2)
                {
                    res.push_back(t);
                }
            }
        }
        return res;
    }

  private:
    std::vector<std::unique_ptr<GameObject>> mGameObjects;
    std::vector<std::unique_ptr<GameObject>> mPendingAdds;
};
