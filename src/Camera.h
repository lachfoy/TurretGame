#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
  public:
    glm::vec2 GetPosition() const { return mPosition; }
    void SetPosition(glm::vec2 position) { mPosition = position; }

    float GetZoom() const { return mZoom; }
    void SetZoom(float zoom) { mZoom = zoom; }

    void SetViewport(int windowWidth, int windowHeight)
    {
        float windowAspect = (float)windowWidth / (float)windowHeight;

        if (windowAspect > kTargetAspect)
        {
            mViewportHeight = windowHeight;
            mViewportWidth = static_cast<int>(windowHeight * kTargetAspect);
            mViewportX = (windowWidth - mViewportWidth) / 2;
            mViewportY = 0;
        }
        else
        {
            mViewportWidth = windowWidth;
            mViewportHeight = static_cast<int>(windowWidth / kTargetAspect);
            mViewportX = 0;
            mViewportY = (windowHeight - mViewportHeight) / 2;
        }
    }

    void GetViewportRect(int& x, int& y, int& width, int& height) const
    {
        x = mViewportX;
        y = mViewportY;
        width = mViewportWidth;
        height = mViewportHeight;
    }

    glm::mat4 GetViewProj() const
    {
        float halfW = kTargetWidth * 0.5f;
        float halfH = kTargetHeight * 0.5f;
        glm::mat4 proj = glm::ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f);

        glm::mat4 view = glm::scale(glm::mat4(1.0f), glm::vec3(mZoom, mZoom, 1.0f));
        view = glm::translate(view, glm::vec3(-mPosition, 0.0f));

        return proj * view;
    }

    glm::vec2 ScreenToWorld(glm::vec2 screenPos) const
    {
        float localX = screenPos.x - mViewportX;
        float localY = screenPos.y - mViewportY;

        float ndcX = (localX / mViewportWidth) * 2.0f - 1.0f;
        float ndcY = 1.0f - (localY / mViewportHeight) * 2.0f;

        glm::vec4 world = glm::inverse(GetViewProj()) * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
        return glm::vec2(world.x, world.y);
    }

  private:
    static constexpr float kTargetWidth = 960.0f;
    static constexpr float kTargetHeight = 540.0f;
    static constexpr float kTargetAspect = kTargetWidth / kTargetHeight;

    glm::vec2 mPosition = glm::vec2(0.0f);
    float mZoom = 3.0f;

    int mViewportX = 0, mViewportY = 0;
    int mViewportWidth = 0, mViewportHeight = 0;
};