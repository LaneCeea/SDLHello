#include <iostream>
#include <array>
#include <vector>

#include <SDL.h>
#include <SDL_image.h>

#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include <Rng/Rng.h>

#include "SDLHandler.h"
#include "Window.h"
#include "Renderer.h"
#include "Texture.h"
#include "Mouse.h"

class Cell {
public:
    Cell() : m_SrcID(4) {
        auto& Rand = lan::RandomInt32<int>::getInstance();
        m_HitBox = { Rand.getNum(0, 900), Rand.getNum(0, 600), 32, 32 };
        m_Number = Rand.getNum(0, 3);
        m_Dst = m_HitBox;
    }

    void handleEvent() {
        auto& mouse = Mouse::getInstance();
        if (mouse.lIsUp()) {
            if (isCollide(mouse.getX(), mouse.getY()))
                m_SrcID = m_Number;
        }
    }

    bool isCollide(int x, int y) const {
        if (x >= m_HitBox.x && x <= m_HitBox.x + m_HitBox.w &&
            y >= m_HitBox.y && y <= m_HitBox.y + m_HitBox.h)
            return true;
        return false;
    }

    constexpr uint8_t getSrcID() const { return m_SrcID; }
    inline const SDL_Rect& getDst() const { return m_Dst; }

private:
    uint8_t m_SrcID;
    SDL_Rect m_Dst;
    SDL_Rect m_HitBox;
    int m_Number;
};

int main(int argc, char *argv[]) {
    
    SDL::init();
    SDL::printVersion();

    {
        Window window("Main", 960, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        
        Renderer renderer = window.createRenderer(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        window.bindRenderer(renderer);
        renderer.printInfo();
        renderer.setAlphaBlending();
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForSDLRenderer(window.data(), renderer.data());
        ImGui_ImplSDLRenderer_Init(renderer.data());
        std::cout << "ImGui: version " << IMGUI_VERSION << '\n';

        Texture texture = renderer.createTexture("res/textures/Tile.png");
        
        {
            int size = 32;
            std::vector<LabeledRect>& Srcs = texture.getSrc();
            Srcs.clear();
            Srcs.reserve(5);
            for (int i = 0; i < 5; ++i) {
                Srcs.emplace_back(0, i * size, 0, size, size);
            }
        }

        std::array<Cell, 100> cells;

        Mouse& mouse = Mouse::getInstance();
        SDL_Event Event;
        bool UserQuit = false;
        while (!UserQuit) {
            while (SDL_PollEvent(&Event)) {
                ImGui_ImplSDL2_ProcessEvent(&Event);
                switch (Event.type) {
                case SDL_QUIT: {
                    UserQuit = true;
                    break;
                }
                case SDL_WINDOWEVENT: {
                    window.handleEvent(Event.window);
                    break;
                }
                }
                
                // decide what to do with this event
            }
            mouse.update();
            // update game state, draw the current frame
            for (auto& cell : cells)
                cell.handleEvent();

            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Frame Rate");
                ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
            ImGui::Render();

            renderer.clear();
            for (const auto& cell : cells)
                renderer.render(texture, texture.getSrc()[cell.getSrcID()].Rect, cell.getDst());
            ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

            renderer.present();
        }

        ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    SDL::quit();
    
    return 0;
}