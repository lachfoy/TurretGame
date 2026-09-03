#pragma once

#include <SDL3/SDL.h>
#include <unordered_set>

class Input
{
  public:
    void BeginFrame()
    {
        mKeyboard.framePressed.clear();
        mGamepad.framePressed.clear();
        mMouse.framePressed.clear();
        mMouse.dx = 0;
        mMouse.dy = 0;
        mMouse.wheelX = 0;
        mMouse.wheelY = 0;
    }

    void HandleEvent(const SDL_Event& event);

    bool IsDown(SDL_Keycode key) const { return mKeyboard.held.find(key) != mKeyboard.held.end(); }

    bool Pressed(SDL_Keycode key) const
    {
        return mKeyboard.framePressed.find(key) != mKeyboard.framePressed.end();
    }

    bool MouseDown(Uint8 button) const { return mMouse.held.count(button); }

    bool MousePressed(Uint8 button) const { return mMouse.framePressed.count(button); }

    int MouseX() const { return mMouse.x; }
    int MouseY() const { return mMouse.y; }

    int MouseDX() const { return mMouse.dx; }
    int MouseDY() const { return mMouse.dy; }

    int MouseWheelX() const { return mMouse.wheelX; }
    int MouseWheelY() const { return mMouse.wheelY; }

    bool IsDown(SDL_GamepadButton button) const { return mGamepad.held.count(button); }
    bool Pressed(SDL_GamepadButton button) const { return mGamepad.framePressed.count(button); }

    float Axis(SDL_GamepadAxis axis) const
    {
        constexpr const float deadZone = 0.2f;

        if (std::fabs(mGamepad.axes[axis]) < deadZone)
            return 0.0f;

        float sign = (mGamepad.axes[axis] > 0.f) ? 1.f : -1.f;
        float scaled = (std::fabs(mGamepad.axes[axis]) - deadZone) / (1.f - deadZone);
        return scaled * sign;
    }

  private:
    struct
    {
        std::unordered_set<SDL_Keycode> held;
        std::unordered_set<SDL_Keycode> framePressed;
    } mKeyboard;

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
    } mMouse;

    struct
    {
        std::unordered_set<SDL_GamepadButton> held;
        std::unordered_set<SDL_GamepadButton> framePressed;
        float axes[SDL_GAMEPAD_AXIS_COUNT]{};
    } mGamepad;
};
