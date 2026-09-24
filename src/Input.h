#pragma once

#include <SDL3/SDL.h>
#include <unordered_set>

class Input
{
  public:
    Input() {}
    ~Input() {}

    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;

    void BeginFrame()
    {
        m_keyboard.framePressed.clear();
        m_gamepad.framePressed.clear();
        m_mouse.framePressed.clear();
        m_mouse.dx = 0;
        m_mouse.dy = 0;
        m_mouse.wheelX = 0;
        m_mouse.wheelY = 0;
    }

    void HandleEvent(const SDL_Event& event);

    bool IsDown(SDL_Keycode key) const
    {
        return m_keyboard.held.find(key) != m_keyboard.held.end();
    }

    bool Pressed(SDL_Keycode key) const
    {
        return m_keyboard.framePressed.find(key) != m_keyboard.framePressed.end();
    }

    bool MouseDown(Uint8 button) const { return m_mouse.held.count(button); }

    bool MousePressed(Uint8 button) const { return m_mouse.framePressed.count(button); }

    int MouseX() const { return m_mouse.x; }
    int MouseY() const { return m_mouse.y; }

    int MouseDX() const { return m_mouse.dx; }
    int MouseDY() const { return m_mouse.dy; }

    int MouseWheelX() const { return m_mouse.wheelX; }
    int MouseWheelY() const { return m_mouse.wheelY; }

    bool IsDown(SDL_GamepadButton button) const { return m_gamepad.held.count(button); }
    bool Pressed(SDL_GamepadButton button) const { return m_gamepad.framePressed.count(button); }

    float Axis(SDL_GamepadAxis axis) const
    {
        constexpr const float deadZone = 0.2f;

        if (std::fabs(m_gamepad.axes[axis]) < deadZone)
            return 0.0f;

        float sign = (m_gamepad.axes[axis] > 0.f) ? 1.f : -1.f;
        float scaled = (std::fabs(m_gamepad.axes[axis]) - deadZone) / (1.f - deadZone);
        return scaled * sign;
    }

  private:
    struct
    {
        std::unordered_set<SDL_Keycode> held;
        std::unordered_set<SDL_Keycode> framePressed;
    } m_keyboard;

    struct
    {
        std::unordered_set<Uint8> held;
        std::unordered_set<Uint8> framePressed;

        int x = 0;
        int y = 0;
        int dx = 0;
        int dy = 0;
        int wheelX = 0;
        int wheelY = 0;
    } m_mouse;

    struct
    {
        std::unordered_set<SDL_GamepadButton> held;
        std::unordered_set<SDL_GamepadButton> framePressed;
        float axes[SDL_GAMEPAD_AXIS_COUNT]{};
    } m_gamepad;
};
