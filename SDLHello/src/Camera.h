#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL_Rect.h>

// 2D camera
class Camera {
public:
    Camera();
    Camera(int win_w, int win_h);

    SDL_Rect transform(const SDL_Rect& Model);
    void transformvScreenToGlobal(int& PosX, int& PosY);

    void onUpdate();

private:
    SDL_FRect m_View, m_Proj;
    float m_CurrentScale;
    float m_ScrollingSensitivity;
    float m_ScalingSensitivity;
    float m_ScalingMin, m_ScalingMax;
};

#endif // !CAMERA_H_