#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include "Entity.h"

class World
{
  public:
    virtual ~World() {}

    virtual void Init() {}
    virtual void Shutdown() {}

    template <typename T, typename... Args> T* CreateEntity(Args&&... args)
    {
        static_assert(std::is_base_of_v<Entity, T>);
        auto e = std::make_unique<T>(std::forward<Args>(args)...);
        e->mWorld = this;
        T* ptr = e.get();
        mEntities.push_back(std::move(e));
        return ptr;
    }

    virtual void Update(float dt)
    {
        for (auto& e : mEntities)
            e->Update(dt);

        mEntities.erase(std::remove_if(mEntities.begin(), mEntities.end(),
                                       [](const auto& e) { return e->IsPendingDestroy(); }),
                        mEntities.end());
    }

    virtual void Render()
    {
        for (auto& e : mEntities)
            e->Render();
    }

  private:
    std::vector<std::unique_ptr<Entity>> mEntities;
};
