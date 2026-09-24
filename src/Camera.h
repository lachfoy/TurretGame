#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
  public:
    glm::vec2 GetPosition() const { return m_position; }
    void SetPosition(glm::vec2 position) { m_position = position; }

    float GetZoom() const { return m_zoom; }
    void SetZoom(float zoom) { m_zoom = zoom; }

    void SetViewport(int windowWidth, int windowHeight)
    {
        float windowAspect = (float)windowWidth / (float)windowHeight;

        if (windowAspect > kTargetAspect)
        {
            m_viewportHeight = windowHeight;
            m_viewportWidth = static_cast<int>(windowHeight * kTargetAspect);
            m_viewportX = (windowWidth - m_viewportWidth) / 2;
            m_viewportY = 0;
        }
        else
        {
            m_viewportWidth = windowWidth;
            m_viewportHeight = static_cast<int>(windowWidth / kTargetAspect);
            m_viewportX = 0;
            m_viewportY = (windowHeight - m_viewportHeight) / 2;
        }
    }

    void GetViewportRect(int& x, int& y, int& width, int& height) const
    {
        x = m_viewportX;
        y = m_viewportY;
        width = m_viewportWidth;
        height = m_viewportHeight;
    }

    glm::mat4 GetViewProj() const
    {
        float halfW = kTargetWidth * 0.5f;
        float halfH = kTargetHeight * 0.5f;
        glm::mat4 proj = glm::ortho(-halfW, halfW, -halfH, halfH, -1.0f, 1.0f);

        glm::mat4 view = glm::scale(glm::mat4(1.0f), glm::vec3(m_zoom, m_zoom, 1.0f));
        view = glm::translate(view, glm::vec3(-m_position, 0.0f));

        return proj * view;
    }

    glm::vec2 ScreenToWorld(glm::vec2 screenPos) const
    {
        float localX = screenPos.x - m_viewportX;
        float localY = screenPos.y - m_viewportY;

        float ndcX = (localX / m_viewportWidth) * 2.0f - 1.0f;
        float ndcY = 1.0f - (localY / m_viewportHeight) * 2.0f;

        glm::vec4 world = glm::inverse(GetViewProj()) * glm::vec4(ndcX, ndcY, 0.0f, 1.0f);
        return glm::vec2(world.x, world.y);
    }

  private:
    static constexpr float kTargetWidth = 960.0f;
    static constexpr float kTargetHeight = 540.0f;
    static constexpr float kTargetAspect = kTargetWidth / kTargetHeight;

    glm::vec2 m_position = glm::vec2(0.0f);
    float m_zoom = 3.0f;

    int m_viewportX = 0, m_viewportY = 0;
    int m_viewportWidth = 0, m_viewportHeight = 0;
};