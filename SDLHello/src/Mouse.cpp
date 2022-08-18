#include "Mouse.h"

#include <SDL_events.h>
#include <SDL_mouse.h>

Mouse::Mouse()
    : m_X(0), m_Y(0),
    m_LIsPressed(false), m_RIsPressed(false),
    m_LIsDown(false), m_RIsDown(false),
    m_LIsUp(false), m_RIsUp(false) {}

Mouse& Mouse::getInstance() {
    static Mouse s_Instance;
    return s_Instance;
}

void Mouse::update() {
    uint32_t state = SDL_GetMouseState(&m_X, &m_Y);
    if ((state & SDL_BUTTON_LMASK) == SDL_BUTTON_LMASK) {   // left button is pressed
        m_LIsDown       = !m_LIsPressed;    // button is down only if it is not pressed last frame
        m_LIsUp         = false;
        m_LIsPressed    = true;
    } else {
        m_LIsDown       = false;
        m_LIsUp         = m_LIsPressed;     // button is up only if it is pressed last frame
        m_LIsPressed    = false;
    }

    if ((state & SDL_BUTTON_RMASK) == SDL_BUTTON_RMASK) { // right button is pressed
        m_RIsDown       = !m_RIsPressed;    // button is down only if it is not pressed last frame
        m_RIsUp         = false;
        m_RIsPressed    = true;
    } else {
        m_RIsDown       = false;
        m_RIsUp         = m_RIsPressed;     // button is up only if it is pressed last frame
        m_RIsPressed    = false;
    }
}