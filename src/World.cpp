
#include "World.h"

void World::Update(float dt)
{
    for (const auto& obj : mGameObjects)
    {
        obj->Update(dt);
    }

    for (auto& obj : mPendingAdds)
    {
        mGameObjects.push_back(std::move(obj));
    }
    mPendingAdds.clear();

    mGameObjects.erase(std::remove_if(mGameObjects.begin(), mGameObjects.end(),
                                      [](const auto& e) { return e->IsPendingDestroy(); }),
                       mGameObjects.end());
}

void World::Render()
{
    for (auto& e : mGameObjects)
        e->Render();
}
