#include "Game.h"

#include <ImGui/imgui.h>

namespace Minesweeper {

Game::Game(Renderer& RenderingContext, int rows, int columns, int mines)
    : RendererPtr(&RenderingContext),
    m_CellTileSet(RendererPtr->createTexture("res/textures/TileSetC.png")), m_BoardPos(40, 60), m_CellSize(40),
    m_TimerDisplay(RenderingContext),
    m_BoardPtr(std::make_unique<GameBoard>(rows, columns, mines)), m_GameHasStarted(false), m_GameHasEnded(EndingType::NONE) {

    {
        auto& Srcs = m_CellTileSet.getSrc();
        int src_cell_size = 384;
        int TileCount = 15;
        Srcs.clear();
        Srcs.reserve(TileCount);
        for (int i = 0; i < TileCount; ++i)
            Srcs.emplace_back(0, i * src_cell_size, 0, src_cell_size, src_cell_size);
    }

}

void Game::onUpdate() {

    // close the cell which was pressed last frame
    static std::vector<Cell*> PressedCell;

    if (m_GameHasEnded != EndingType::NONE)
        return;

    for (auto& CellPtr : PressedCell) {
        if (CellPtr) {
            CellPtr->state = CellState::CLOSED;
        } else break;
    }
    PressedCell.clear();

    const auto& Mouse = Mouse::getInstance();
    lan::Vec2<int> MousePos(Mouse.getX(), Mouse.getY());
    RendererPtr->getCamera()->transform(MousePos.x, MousePos.y);
    {
        lan::Vec2<int> InteractedCoord = (MousePos - m_BoardPos) / m_CellSize;
        if (m_BoardPtr->isInRange(InteractedCoord)) {
            auto& InteractedCell = m_BoardPtr->at(InteractedCoord);

            if (!m_GameHasStarted) {
                // first click
                if (Mouse.lIsUp()) {
                    m_BoardPtr->adjust(InteractedCoord);
                    m_BoardPtr->open(InteractedCoord);
                    m_TimerDisplay.run();
                    m_GameHasStarted = true;
                }
                return;
            }
            bool lIsPressed = Mouse.lIsPressed();
            bool rIsPressed = Mouse.rIsPressed();
            bool rIsDown    = Mouse.rIsDown();
            bool lIsUp      = Mouse.lIsUp();
            bool rIsUp      = Mouse.rIsUp();

            if (lIsPressed) {
                if (m_BoardPtr->press(InteractedCoord) == Interaction::SUCCESS)
                    PressedCell.push_back(&InteractedCell);

                if (rIsPressed) { // press the chording area
                    PressedCell.reserve(9);
                    m_BoardPtr->forEachNeighbor(
                        InteractedCoord,
                        [this, &InteractedCoord, &InteractedCell](const lan::Vec2<int32_t>& NeighborCoord) {
                            if (m_BoardPtr->press(NeighborCoord) == Interaction::SUCCESS)
                                PressedCell.push_back(&m_BoardPtr->at(NeighborCoord));
                        }
                    );
                }
            } else if (rIsDown) {
                m_BoardPtr->flag(InteractedCoord);
            }
            Interaction CheckExploded = Interaction::NONE;

            if ((lIsPressed && rIsUp) || (lIsUp && rIsPressed) || (lIsUp && rIsUp))
                CheckExploded = m_BoardPtr->chording(InteractedCoord);
            else if (lIsUp)
                CheckExploded = m_BoardPtr->open(InteractedCoord);

            if (CheckExploded == Interaction::EXPLODED)
                gameLose();
            
        }
        checkWinning();
    }


}

void Game::onImGuiRender() {
    {
        ImGui::Begin("Minesweeper");
        if (ImGui::Button("Reveal Board"))
            reveal();
        else if (ImGui::Button("Beginner"))
            newGame(9, 9, 10);
        else if (ImGui::Button("Intermediate"))
            newGame(16, 16, 40);
        else if (ImGui::Button("Expert"))
            newGame(16, 30, 99);
        ImGui::End();
    }

}

void Game::onRender() {
    int offset = 0;
    SDL_Rect Dst = { 0, 0, m_CellSize, m_CellSize };
    for (const auto& CurrentCell : *m_BoardPtr) {
        const lan::Vec2<int> CurrentCoord = m_BoardPtr->offsetToCoord(offset);
        Dst.x = m_BoardPos.x + CurrentCoord.x * m_CellSize;
        Dst.y = m_BoardPos.y + CurrentCoord.y * m_CellSize;
        if (CurrentCell.state == CellState::OPENED) {
            if (CurrentCell.hasMine)
                RendererPtr->render(m_CellTileSet, m_CellTileSet.getSrc()[(int)CellState::OPENED + Cell::MINE].Rect, Dst);
            else
                RendererPtr->render(m_CellTileSet, m_CellTileSet.getSrc()[(int)CellState::OPENED + CurrentCell.number].Rect, Dst);
        } else {
            RendererPtr->render(m_CellTileSet, m_CellTileSet.getSrc()[(int)CurrentCell.state].Rect, Dst);
        }
        ++offset;
    }
    m_TimerDisplay.onRender();
}

void Game::checkWinning() {
    if (m_GameHasEnded != EndingType::NONE)
        return;

    for (const auto& Cell : *m_BoardPtr) {
        // if any non-mine cell is not opened, the game is still on
        if (!Cell.hasMine && Cell.state != CellState::OPENED)
            return;
    }

    DEBUGLOG("YOUVE WON\n");
    m_GameHasEnded = EndingType::WIN;
    reveal();
}

void Game::gameLose() {
    DEBUGLOG("GAME OVER\n");
    m_GameHasEnded = EndingType::LOSE;
    reveal();
}

void Game::reveal() {
    if (m_GameHasEnded == EndingType::WIN) {
        for (auto& Cell : *m_BoardPtr) {
            if (Cell.hasMine && Cell.state != CellState::EXPLODED)
                Cell.state = CellState::FLAGED;
        }
    } else {
        for (auto& Cell : *m_BoardPtr) {
            if (Cell.hasMine && Cell.state != CellState::FLAGED && Cell.state != CellState::EXPLODED)
                Cell.state = CellState::OPENED;
            else if (!Cell.hasMine && Cell.state == CellState::FLAGED)
                Cell.state = CellState::WRONGFLAGED;
        }
    }
    m_TimerDisplay.pause();
}

void Game::newGame(int rows, int columns, int mines) {
    m_BoardPtr = std::make_unique<GameBoard>(rows, columns, mines);
    m_TimerDisplay.reset();
    m_GameHasStarted = false;
    m_GameHasEnded = EndingType::NONE;
}

} // namespace Minesweeper