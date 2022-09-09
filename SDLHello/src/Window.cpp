#include "Window.h"

#include <SDL_events.h>
#include <SDL_surface.h>
#include <SDL_video.h>

#include "SDLErrorHandler.h"

const char* Window::DEFAULT_TITLE = "Title";
const int Window::DEFAULT_WIDTH = 640;
const int Window::DEFAULT_HEIGHT = 640;

Window::Window()
    : m_Data(nullptr), m_Surface(nullptr), m_Renderer(nullptr),
    m_Title(DEFAULT_TITLE), m_Width(DEFAULT_WIDTH), m_Height(DEFAULT_HEIGHT) {
    DEBUGLOG("[SDL] Creating Window...\n");
    init(SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
}

Window::Window(const char* title, int w, int h, uint32_t flags)
    : m_Data(nullptr), m_Surface(nullptr), m_Title(title), m_Width(w), m_Height(h) {
    DEBUGLOG("[SDL] Creating Window...\n");
    init(flags);
}

Window::~Window() {
    destroy();
}

void Window::init(uint32_t flags) {
    m_Data = SDL_CreateWindow(
        m_Title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        m_Width, m_Height,
        flags
    );
    SDLAssert(m_Data != nullptr);
    
    m_Surface = SDL_GetWindowSurface(m_Data);
    SDLAssert(m_Surface != nullptr);
}

void Window::destroy() {
    SDL_DestroyWindow(m_Data);
}

Renderer Window::createRenderer(uint32_t flags) {
    DEBUGLOG("[SDL] Creating Renderer...\n");

    SDL_Renderer* RendererData = SDL_CreateRenderer(m_Data, -1, flags);
    SDLAssert(RendererData != nullptr);
    return Renderer(RendererData);
}

void Window::bindRenderer(Renderer& RenderingContext) {
    m_Renderer = &RenderingContext;
}

void Window::handleEvent(const SDL_WindowEvent& Event) {
    switch (Event.event) {
    case SDL_WINDOWEVENT_EXPOSED: {
        
        break;
    }
    case SDL_WINDOWEVENT_RESIZED: {
        m_Width  = Event.data1;
        m_Height = Event.data2;
        break;
    }
    }
}