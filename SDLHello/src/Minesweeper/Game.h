#ifndef MINESWEEPER_GAME_H_
#define MINESWEEPER_GAME_H_

#include <memory>

#include "Mouse.h"
#include "Renderer.h"
#include "Texture.h"

#include "GameBoard.h"
#include "Vec2.h"

namespace Minesweeper {

enum class EndingType {
    NONE = 0, LOSE, WIN
};

class Game {
public:
    Game(Renderer& RenderingContext, int rows, int columns, int mines);

    void onUpdate();
    void onRender(Renderer& RenderingContext);

    // Events

    void checkWinning();
    void gameLose();
    void reveal();
    void newGame(int rows, int columns, int mines);

private:
    // rendering data
    Texture m_TileSet;
    Vec2<int> m_BoardPos;
    int m_CellSize;

    // game data
    std::unique_ptr<GameBoard> m_BoardPtr;
    bool m_GameHasStarted;
    EndingType m_GameHasEnded;
};

} // namespace Minesweeper

#endif // !MINESWEEPER_GAME_H_