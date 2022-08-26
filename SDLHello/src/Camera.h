#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL_Rect.h>

// 2D camera
class Camera {
public:
    Camera();

    SDL_Rect transform(const SDL_Rect& Model);
    void transform(int& PosX, int& PosY);

    void onUpdate();

private:
    struct PreciseRect {
        float x, y;
        float w, h;
    };
    // (x, y) : [Origin] camera's up left corner coordinate
    // (w, h) : [Scaler] camera's dimension / window's dimension
    PreciseRect m_View;
    float m_ScrollingSensitivity;
    float m_ScalingSensitivity;
    float m_ScalingMin, m_ScalingMax;
};

#endif // !CAMERA_H_