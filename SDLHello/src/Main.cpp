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

#include "Minesweeper/Game.h"

int main(int argc, char *argv[]) {
    
    SDL::init();
    SDL::printVersion();

    {
        Window window("Main", 960, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        
        Renderer renderer = window.createRenderer(SDL_RENDERER_ACCELERATED/* | SDL_RENDERER_PRESENTVSYNC*/);
        window.bindRenderer(renderer);
        renderer.printInfo();
        renderer.setAlphaBlending();
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplSDL2_InitForSDLRenderer(window.data(), renderer.data());
        ImGui_ImplSDLRenderer_Init(renderer.data());
        std::cout << "ImGui: version " << IMGUI_VERSION << '\n';

        Minesweeper::Game game(renderer, 9, 9, 10);

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
            game.onUpdate();
            // update game state, draw the current frame

            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();
            {
                ImGui::Begin("Frame Rate");
                ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
            {
                ImGui::Begin("Debug");
                if (ImGui::Button("Reveal Board"))
                    game.reveal();
                if (ImGui::Button("New Game"))
                    game.newGame(9, 9, 10);
                ImGui::End();
            }
            ImGui::Render();

            renderer.clear();
            game.onRender(renderer);
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