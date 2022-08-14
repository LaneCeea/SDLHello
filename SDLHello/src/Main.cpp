#include <iostream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_sdlrenderer.h>

#include "SDLHandler.h"
#include "Window.h"
#include "Renderer.h"
#include "Texture.h"

int main(int argc, char *argv[]) {
    
    SDL::init();
    SDL::printVersion();

    {
        Window window("Main", 640, 640);
        
        Renderer renderer(window);
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
            Srcs.reserve(4);
            for (int i = 0; i < 4; ++i) {
                Srcs.emplace_back(0, i * size, 0, size, size);
            }
            std::vector<LabeledRect>& Dsts = texture.getDst();
            Dsts.clear();
            Dsts.reserve(10);
            for (int id = 0; id < 4; ++id) {
                for (int i = 0; i < 10; ++i) {
                    Dsts.emplace_back(id, i * size, id * size, size, size);
                }
            }
        }

        bool UserQuit = false;
        while (!UserQuit) {
            SDL_Event Event;
            while (SDL_PollEvent(&Event)) {
                ImGui_ImplSDL2_ProcessEvent(&Event);
                if (Event.type == SDL_QUIT) {
                    UserQuit = true;
                    break;
                }
                // decide what to do with this event
            }
            // update game state, draw the current frame

            ImGui_ImplSDLRenderer_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            {
                ImGui::Begin("Frame Rate");
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
            ImGui::Render();

            renderer.clear();
            renderer.render(texture);
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