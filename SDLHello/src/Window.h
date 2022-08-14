#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL_video.h>
#include <SDL_surface.h>

class Window {
public:
    Window();
    Window(const char* title, int w, int h);
    ~Window();

    void init();
    void destroy();

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
    const char* m_Title;
    int m_Width;
    int m_Height;
};

#endif // !WINDOW_H_