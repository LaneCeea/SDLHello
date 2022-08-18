#ifndef MOUSE_H_
#define MOUSE_H_

#include <SDL_events.h>

class Mouse {
public:
    static Mouse& getInstance();

    constexpr int getX() const {
        return m_X;
    }

    constexpr int getY() const {
        return m_Y;
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

    void update();

public:
    Mouse(const Mouse&)             = delete;
    Mouse& operator=(const Mouse&)  = delete;

private:
    Mouse();

private:
    int m_X, m_Y;
    bool m_LIsPressed;
    bool m_RIsPressed;
    bool m_LIsDown;
    bool m_RIsDown;
    bool m_LIsUp;
    bool m_RIsUp;
};

#endif // !MOUSE_H_