#include "Game.h"

namespace Minesweeper {

Game::Game(Renderer& RenderingContext, int rows, int columns, int mines)
    : m_TileSet(RenderingContext.createTexture("res/textures/TileSetA.png")),
    m_BoardPos(50, 50), m_CellSize(32),
    m_BoardPtr(std::make_unique<GameBoard>(rows, columns, mines)), m_GameHasStarted(false), m_GameHasEnded(EndingType::NONE) {

    {
        auto& Srcs = m_TileSet.getSrc();
        int src_cell_size = 64;
        int TileCount = 14;
        Srcs.clear();
        Srcs.reserve(TileCount);
        for (int i = 0; i < TileCount; ++i)
            Srcs.emplace_back(0, i * src_cell_size, 0, src_cell_size, src_cell_size);
    }

}

void Game::onUpdate() {

    // close the cell which was pressed last frame
    static std::vector<Cell*> PressedCell(9, nullptr);

    if (m_GameHasEnded != EndingType::NONE)
        return;

    for (auto& CellPtr : PressedCell) {
        if (CellPtr) {
            CellPtr->state = CellState::CLOSED;
            CellPtr = nullptr;
        } else break;
    }

    const auto& Mouse = Mouse::getInstance();
    Vec2<int> MousePos(Mouse.getX(), Mouse.getY());
    {
        Vec2<int> InteractedCoord = (MousePos - m_BoardPos) / m_CellSize;
        if (m_BoardPtr->isInRange(InteractedCoord)) {
            auto& InteractedCell = m_BoardPtr->at(InteractedCoord);

            if (!m_GameHasStarted) {
                // first click
                if (Mouse.lIsUp()) {
                    m_BoardPtr->adjust(InteractedCoord);
                    m_BoardPtr->open(InteractedCoord);
                    m_GameHasStarted = true;
                }
                return;
            }

            if (Mouse.lIsPressed()) {
                if (m_BoardPtr->press(InteractedCoord) == Interaction::SUCCESS)
                    PressedCell.front() = &InteractedCell;
            } else if (Mouse.rIsDown()) {
                m_BoardPtr->flag(InteractedCoord);
            } else if (Mouse.lIsUp()) {
                Interaction CheckExploded = Interaction::NONE;
                if (Mouse.rIsUp())
                    CheckExploded = m_BoardPtr->chording(InteractedCoord);
                else
                    CheckExploded = m_BoardPtr->open(InteractedCoord);
                if (CheckExploded == Interaction::EXPLODED)
                    gameLose();
            }
        }
        checkWinning();
    }
}

void Game::onRender(Renderer& RenderingContext) {
    int offset = 0;
    SDL_Rect Dst = { 0, 0, m_CellSize, m_CellSize };
    for (const auto& CurrentCell : *m_BoardPtr) {
        const Vec2<int> CurrentCoord = m_BoardPtr->offsetToCoord(offset);
        Dst.x = m_BoardPos.x + CurrentCoord.x * m_CellSize;
        Dst.y = m_BoardPos.y + CurrentCoord.y * m_CellSize;
        if (CurrentCell.state == CellState::OPENED) {
            if (CurrentCell.hasMine)
                RenderingContext.render(m_TileSet, m_TileSet.getSrc()[(int)CellState::OPENED + Cell::MINE].Rect, Dst);
            else
                RenderingContext.render(m_TileSet, m_TileSet.getSrc()[(int)CellState::OPENED + CurrentCell.number].Rect, Dst);
        } else {
            RenderingContext.render(m_TileSet, m_TileSet.getSrc()[(int)CurrentCell.state].Rect, Dst);
        }
        ++offset;
    }
    
}

void Game::checkWinning() {
    if (m_GameHasEnded != EndingType::NONE)
        return;

    for (const auto& Cell : *m_BoardPtr) {
        // if any non-mine cell is not opened, the game is still on
        if (!Cell.hasMine && Cell.state != CellState::OPENED)
            return;
    }

    DEBUGLOG("YOUVE WON");
    m_GameHasEnded = EndingType::WIN;
    reveal();
}

void Game::gameLose() {
    DEBUGLOG("GAME OVER");
    m_GameHasEnded = EndingType::LOSE;
    reveal();
}

void Game::reveal() {
    CellState MineState;
    if (m_GameHasEnded == EndingType::WIN)
        MineState = CellState::FLAGED;
    else
        MineState = CellState::OPENED;

    for (auto& Cell : *m_BoardPtr) {
        if (Cell.hasMine && Cell.state != CellState::EXPLODED)
            Cell.state = MineState;
    }

}

void Game::newGame(int rows, int columns, int mines) {
    m_BoardPtr = std::make_unique<GameBoard>(rows, columns, mines);
    m_GameHasStarted = false;
    m_GameHasEnded = EndingType::NONE;
}

} // namespace Minesweeper