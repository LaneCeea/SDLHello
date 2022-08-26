#ifndef MINESWEEPER_TIMERDISPLAY_H_
#define MINESWEEPER_TIMERDISPLAY_H_

#include "Timer.h"

#include "Renderer.h"
#include "Texture.h"

namespace Minesweeper {

class TimerDisplay : public Timer {
public:
    TimerDisplay(Renderer& RenderingContext);

    void onRender();

public:
    static constexpr uint64_t DISPLAY_NUM_MAX = 999;
    static constexpr uint64_t DISPLAY_DIG_MAX = 3;

private:
    // rendering data
    Renderer* RendererPtr;
    Texture m_NumberTexture;
};

} // namespace Minesweeper

#endif // !MINESWEEPER_TIMERDISPLAY_H_