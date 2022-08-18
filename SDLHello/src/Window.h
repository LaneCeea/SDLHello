#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL_events.h>
#include <SDL_surface.h>
#include <SDL_video.h>

#include "Renderer.h"

class Window {
public:
    Window();
    Window(const char* title, int w, int h, uint32_t flags);
    ~Window();

    void init(uint32_t flags);
    void destroy();

    Renderer createRenderer(uint32_t flags);
    void bindRenderer(Renderer& RenderingContext);

    void handleEvent(const SDL_WindowEvent& Event);


    ///////////////////////////
    // Getter
    ///////////////////////////

    inline SDL_Window* data() {
        return m_Data;
    }

    inline SDL_Surface* getSurface() {
        return m_Surface;
    }

    constexpr int getWidth() const {
        return m_Width;
    }

    constexpr int getHeight() const {
        return m_Height;
    }


public:
    static const char* DEFAULT_TITLE;
    static const int DEFAULT_WIDTH;
    static const int DEFAULT_HEIGHT;

private:
    SDL_Window* m_Data;
    SDL_Surface* m_Surface;
    Renderer* m_Renderer;
    const char* m_Title;
    int m_Width;
    int m_Height;
};

#endif // !WINDOW_H_