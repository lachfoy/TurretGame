#include "Input.h"

#include <algorithm>

void Input::HandleEvent(const SDL_Event& event)
{
    // Keyboard
    if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat)
    {
        m_keyboard.held.insert(event.key.key);
        m_keyboard.framePressed.insert(event.key.key);
    }
    else if (event.type == SDL_EVENT_KEY_UP)
    {
        m_keyboard.held.erase(event.key.key);
    }

    // Mouse
    switch (event.type)
    {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            m_mouse.held.insert(event.button.button);
            m_mouse.framePressed.insert(event.button.button);
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            m_mouse.held.erase(event.button.button);
            break;
        case SDL_EVENT_MOUSE_MOTION:
            m_mouse.x = event.motion.x;
            m_mouse.y = event.motion.y;
            m_mouse.dx += event.motion.xrel;
            m_mouse.dy += event.motion.yrel;
            break;
        case SDL_EVENT_MOUSE_WHEEL:
            m_mouse.wheelX += event.wheel.x;
            m_mouse.wheelY += event.wheel.y;
            break;
    }

    // Gamepad
    switch (event.type)
    {
        case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
            m_gamepad.held.insert((SDL_GamepadButton)event.gbutton.button);
            m_gamepad.framePressed.insert((SDL_GamepadButton)event.gbutton.button);
            break;
        case SDL_EVENT_GAMEPAD_BUTTON_UP:
            m_gamepad.held.erase((SDL_GamepadButton)event.gbutton.button);
            break;
        case SDL_EVENT_GAMEPAD_AXIS_MOTION:
            constexpr float inv = 1.0f / 32768.0f;
            m_gamepad.axes[event.gaxis.axis] = std::clamp(event.gaxis.value * inv, -1.f, 1.f);
            break;
    }
}
