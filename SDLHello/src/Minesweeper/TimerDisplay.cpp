#include "TimerDisplay.h"

#include <string>
#include <ImGui/imgui.h>

namespace Minesweeper {

TimerDisplay::TimerDisplay(Renderer& RenderingContext) 
    : Timer(), RendererPtr(&RenderingContext),
    m_NumberTexture(RendererPtr->createTexture("res/textures/SevenSegmentDisplay.png")) {

    const int SpriteCount = 11;
    const int w = m_NumberTexture.getWidth() / SpriteCount;
    const int h = m_NumberTexture.getHeight();
    {
        auto& Srcs = m_NumberTexture.getSrc();
        Srcs.clear();
        Srcs.reserve(SpriteCount);
        for (int i = 0; i < SpriteCount; ++i)
            Srcs.emplace_back(0, i * w, 0, w, h);
    }
    {
        auto& Dsts = m_NumberTexture.getDst();
        SDL_Rect Dst = { 40, 0, w / 20, h / 20 };
        const int interval = Dst.w / 8;
        Dsts.clear();
        Dsts.reserve(DISPLAY_DIG_MAX);
        for (int i = 0; i < DISPLAY_DIG_MAX; ++i) {
            Dsts.emplace_back(0, Dst);
            Dst.x += Dst.w + interval;
        }
    }
    
}

void TimerDisplay::onRender() {
    std::string DisplayTime;
    {
        const uint64_t CurrentTime = static_cast<uint64_t>(now());
        std::string Temp = std::to_string(std::min(CurrentTime, DISPLAY_NUM_MAX));
        switch (Temp.size()) {
        case 1:
            DisplayTime = "00" + Temp;
            break;

        case 2:
            DisplayTime = "0" + Temp;
            break;

        default:
            DisplayTime = Temp;
            break;
        }
    }
    auto& Dsts = m_NumberTexture.getDst();
    for (int i = 0; i < DISPLAY_DIG_MAX; ++i) {
        Dsts[i].SrcID = 1 + DisplayTime[i] - '0';
    }
    RendererPtr->render(m_NumberTexture);
}



} // namespace Minesweeper