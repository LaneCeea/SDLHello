#ifndef MOUSE_H_
#define MOUSE_H_

#include <SDL_events.h>

class Mouse {
public:
    static Mouse& getInstance();

    void handleWheelEvent(const SDL_MouseWheelEvent& Event);
    void onUpdate();

    constexpr int getX() const {
        return m_PosX;
    }

    constexpr int getY() const {
        return m_PosY;
    }

    constexpr float getScrollAmount() const {
        return m_ScrollAmount;
    }

    constexpr bool lIsPressed() const {
        return m_LIsPressed;
    }

    constexpr bool rIsPressed() const {
        return m_RIsPressed;
    }

    constexpr bool lIsDown() const {
        return m_LIsDown;
    }

    constexpr bool rIsDown() const {
        return m_RIsDown;
    }

    constexpr bool lIsUp() const {
        return m_LIsUp;
    }

    constexpr bool rIsUp() const {
        return m_RIsUp;
    }

public:
    Mouse(const Mouse&)             = delete;
    Mouse& operator=(const Mouse&)  = delete;

private:
    Mouse();

private:
    int m_PosX, m_PosY;
    float m_ScrollAmount; // The amount scrolled vertically, positive away from the user and negative toward the user
    bool m_HasWheelEvent;
    bool m_LIsPressed, m_LIsDown, m_LIsUp;
    bool m_RIsPressed, m_RIsDown, m_RIsUp;
};

#endif // !MOUSE_H_