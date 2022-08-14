#include "Window.h"

#include <SDL_video.h>
#include <SDL_surface.h>

#include "SDLErrorHandler.h"

const char* Window::DEFAULT_TITLE = "Title";
const int Window::DEFAULT_WIDTH = 640;
const int Window::DEFAULT_HEIGHT = 640;

Window::Window()
    : m_Data(nullptr), m_Surface(nullptr),
    m_Title(DEFAULT_TITLE), m_Width(DEFAULT_WIDTH), m_Height(DEFAULT_HEIGHT) {
    init();
}

Window::Window(const char* title, int w, int h)
    : m_Data(nullptr), m_Surface(nullptr), m_Title(title), m_Width(w), m_Height(h) {
    init();
}

Window::~Window() {
    destroy();
}

void Window::init() {
    m_Data = SDL_CreateWindow(
        m_Title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        m_Width, m_Height,
        SDL_WINDOW_SHOWN
    );
    SDLAssert(m_Data != nullptr);
    
    m_Surface = SDL_GetWindowSurface(m_Data);
    SDLAssert(m_Surface != nullptr);
}

void Window::destroy() {
    SDL_DestroyWindow(m_Data);
}