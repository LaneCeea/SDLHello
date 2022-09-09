#include "Camera.h"

#include <SDL_keyboard.h>
#include "Mouse.h"

Camera::Camera()
    : m_View{ 0.0f, 0.0f, 640.0f, 640.0f }, m_Proj{ 0.0f, 0.0f, 640.0f, 640.0f },
    m_CurrentScale(1.0f),
    m_ScrollingSensitivity(20.0f), m_ScalingSensitivity(0.125f),
    m_ScalingMin(0.25f), m_ScalingMax(5.0f) {}

Camera::Camera(int win_w, int win_h)
    : m_View{ 0.0f, 0.0f, static_cast<float>(win_w), static_cast<float>(win_h) },
    m_Proj{ 0.0f, 0.0f, static_cast<float>(win_w), static_cast<float>(win_h) },
    m_CurrentScale(1.0f),
    m_ScrollingSensitivity(20.0f), m_ScalingSensitivity(0.125f),
    m_ScalingMin(0.25f), m_ScalingMax(5.0f) {}

SDL_Rect Camera::transform(const SDL_Rect& Model) {
    float x = (static_cast<float>(Model.x) - m_View.x) / m_View.w * m_Proj.w;
    float y = (static_cast<float>(Model.y) - m_View.y) / m_View.h * m_Proj.h;
    float w = static_cast<float>(Model.w) / m_View.w * m_Proj.w;
    float h = static_cast<float>(Model.h) / m_View.h * m_Proj.h;
    return SDL_Rect { 
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(w),
        static_cast<int>(h)
    };
}

void Camera::transformvScreenToGlobal(int& PosX, int& PosY) {
    PosX = static_cast<int>((static_cast<float>(PosX) / m_Proj.w * m_View.w) + m_View.x);
    PosY = static_cast<int>((static_cast<float>(PosY) / m_Proj.h * m_View.h) + m_View.y);
}

void Camera::onUpdate() {
    auto& Mouse = Mouse::getInstance();
    auto KeyboardState = SDL_GetKeyboardState(nullptr);
    if (KeyboardState[SDL_SCANCODE_LCTRL]) {
        float scale = m_CurrentScale + m_ScalingSensitivity * (-Mouse.getScrollAmount());
        if (scale >= m_ScalingMin && scale <= m_ScalingMax) {
            m_CurrentScale = scale;
            m_View.w = m_CurrentScale * m_Proj.w;
            m_View.h = m_CurrentScale * m_Proj.h;
        }
    } else {
        m_View.y -= m_ScrollingSensitivity * Mouse.getScrollAmount();
    }
}
