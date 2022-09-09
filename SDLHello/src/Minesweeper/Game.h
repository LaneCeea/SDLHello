#ifndef MINESWEEPER_GAME_H_
#define MINESWEEPER_GAME_H_

#include <memory>
#include <Vec/Vec2.h>

#include "Mouse.h"
#include "Renderer.h"
#include "Texture.h"

#include "GameBoard.h"
#include "TimerDisplay.h"

namespace Minesweeper {

enum class EndingType {
    NONE = 0, WIN, LOSE
};

class Game {
public:
    Game(Renderer& RenderingContext, int rows, int columns, int mines);

    void onUpdate();
    void onImGuiRender();
    void onRender();

    // Events

    void checkWinning();
    void gameLose();
    void reveal();
    void newGame();
    void newGame(int rows, int columns, int mines);

private:
    void _MainGameUpdate(const lan::Vec2<int>& MouseWorldPos);
    void _FaceUpdate(const lan::Vec2<int>& MouseWorldPos);

    void _MainGameRender();
    void _FaceRender();

private:
    enum class TileSet {
        CLOSED = 0,
        FLAGGED, WRONG_FLAGGED,
        PRESSED,
        ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
        MINE, EXPLODED_MINE,
        FACE_DEFAULT, FACE_WIN, FACE_LOSE,
        TILE_SET_TOTAL
    };

private:
    // rendering data
    Renderer* RendererPtr;

    Texture m_CellTileSet;
    int m_CellSize;
    lan::Vec2<int> m_BoardPos;
    int m_FaceSize;
    lan::Vec2<int> m_FacePos;

    TimerDisplay m_TimerDisplay;

    // game data
    std::unique_ptr<GameBoard> m_BoardPtr;
    bool m_GameHasStarted;
    EndingType m_GameHasEnded;
};

} // namespace Minesweeper

#endif // !MINESWEEPER_GAME_H_