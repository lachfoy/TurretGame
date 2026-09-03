#include "Input.h"

#include <algorithm>

void Input::HandleEvent(const SDL_Event& event)
{
    // Keyboard
    if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
    {
        mKeyboard.held.insert(event.key.key);
        mKeyboard.framePressed.insert(event.key.key);
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        mKeyboard.held.erase(event.key.key);
    }

    // Mouse
    switch (event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            mMouse.held.insert(event.button.button);
            mMouse.framePressed.insert(event.button.button);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            mMouse.held.erase(event.button.button);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            mMouse.x = event.motion.x;
            mMouse.y = event.motion.y;
            mMouse.dx += event.motion.xrel;
            mMouse.dy += event.motion.yrel;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            mMouse.wheelX += event.wheel.x;
            mMouse.wheelY += event.wheel.y;
            break;
    }

    // Gamepad
    switch (event.type)
    {
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            mGamepad.held.insert((SDL_GamepadButton)event.gbutton.button);
            mGamepad.framePressed.insert((SDL_GamepadButton)event.gbutton.button);
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
            mGamepad.held.erase((SDL_GamepadButton)event.gbutton.button);
            break;
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            constexpr float inv = 1.0f / 32768.0f;
            mGamepad.axes[event.gaxis.axis] = std::clamp(event.gaxis.value * inv, -1.f, 1.f);
            break;
    }
}
