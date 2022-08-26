#ifndef RENDERER_H_
#define RENDERER_H_

#include <vector>
#include <SDL_render.h>
#include <SDL_pixels.h>

#include "Texture.h"
#include "Camera.h"

class Renderer {
public:
    Renderer(SDL_Renderer* data);
    ~Renderer();

    void destroy();

    inline SDL_Renderer* data() {
        return m_Data;
    }

    void printInfo();

    Texture createTexture(const char* path);
    
    inline void setCamera(Camera& BindingCamera) {
        m_CameraPtr = &BindingCamera;
    }

    inline Camera* getCamera() {
        return m_CameraPtr;
    }

    void setAlphaBlending();
    void setColor(const SDL_Color& Color);
    void clear();

    void render(Texture& SourceTexture, const SDL_Rect& Src, const SDL_Rect& Dst);
    void render(Texture& SourceTexture, const std::vector<LabeledRect>& Srcs, const std::vector<LabeledRect>& Dsts);
    void render(Texture& SourceTexture);
    
    void present();

private:
    SDL_Renderer* m_Data;
    Camera* m_CameraPtr;
};

#endif // !RENDERER_H_