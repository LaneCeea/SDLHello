#include "Game.h"

#include <ImGui/imgui.h>

namespace Minesweeper {

Game::Game(Renderer& RenderingContext, int rows, int columns, int mines)
    : RendererPtr(&RenderingContext),
    m_CellTileSet(RendererPtr->createTexture("res/textures/TileSetC.png")),
    m_CellSize(36), m_BoardPos(40, 80),
    m_FaceSize(80), m_FacePos(m_BoardPos + lan::Vec2<int>(columns * m_CellSize / 2 - m_FaceSize / 2, -m_FaceSize)),
    m_TimerDisplay(RenderingContext),
    m_BoardPtr(std::make_unique<GameBoard>(rows, columns, mines)), m_GameHasStarted(false), m_GameHasEnded(EndingType::NONE) {

    {
        auto& Srcs = m_CellTileSet.getSrc();
        int src_cell_size = 384;
        int TileCount = static_cast<int>(TileSet::TILE_SET_TOTAL);
        Srcs.clear();
        Srcs.reserve(TileCount);
        for (int i = 0; i < TileCount; ++i)
            Srcs.emplace_back(0, i * src_cell_size, 0, src_cell_size, src_cell_size);
    }

}

void Game::onUpdate() {

    const auto& Mouse = Mouse::getInstance();
    lan::Vec2<int> MousePos(Mouse.getX(), Mouse.getY());
    RendererPtr->getCamera()->transformvScreenToGlobal(MousePos.x, MousePos.y);

    _MainGameUpdate(MousePos);
    _FaceUpdate(MousePos);

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
    _MainGameRender();
    _FaceRender();
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

    DEBUGLOG("[Minesweeper] YOU WIN\n");
    m_GameHasEnded = EndingType::WIN;
    reveal();
}

void Game::gameLose() {
    DEBUGLOG("[Minesweeper] GAME OVER\n");
    m_GameHasEnded = EndingType::LOSE;
    reveal();
}

void Game::reveal() {
    DEBUGLOG("[Minesweeper] Reveal GameBoard!\n");
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

void Game::newGame() {
    newGame(m_BoardPtr->ROWS, m_BoardPtr->COLUMNS, m_BoardPtr->MINES);
}

void Game::newGame(int rows, int columns, int mines) {
    DEBUGLOG("[Minesweeper] New Game!\n");
    m_BoardPtr = std::make_unique<GameBoard>(rows, columns, mines);
    m_FacePos = m_BoardPos + lan::Vec2<int>(columns * m_CellSize / 2 - m_FaceSize / 2, -m_FaceSize);
    m_TimerDisplay.reset();
    m_GameHasStarted = false;
    m_GameHasEnded = EndingType::NONE;
}

void Game::_MainGameUpdate(const lan::Vec2<int>& MouseWorldPos) {
    // close the cell which was pressed last frame
    static std::vector<Cell*> PressedCell;
    for (auto& CellPtr : PressedCell) {
        if (CellPtr) {
            CellPtr->state = CellState::CLOSED;
        } else break;
    }
    PressedCell.clear();

    const auto& Mouse = Mouse::getInstance();
    bool lIsPressed   = Mouse.lIsPressed();
    bool rIsPressed   = Mouse.rIsPressed();
    bool rIsDown      = Mouse.rIsDown();
    bool lIsUp        = Mouse.lIsUp();
    bool rIsUp        = Mouse.rIsUp();

    lan::Vec2<int> InteractedCoord = (MouseWorldPos - m_BoardPos) / m_CellSize;
    if (m_GameHasEnded != EndingType::NONE || !m_BoardPtr->isInRange(InteractedCoord))
        return;
    
    auto& InteractedCell = m_BoardPtr->at(InteractedCoord);

    if (lIsPressed) {
        if (m_BoardPtr->press(InteractedCoord) == Interaction::SUCCESS)
            PressedCell.push_back(&InteractedCell);

        if (rIsPressed) { // press the chording area
            PressedCell.reserve(9);
            m_BoardPtr->forEachNeighbor(
                InteractedCoord,
                [this, &InteractedCoord, &InteractedCell](const lan::Vec2<int32_t>& NeighborCoord) -> void {
                    if (m_BoardPtr->press(NeighborCoord) == Interaction::SUCCESS)
                        PressedCell.push_back(&m_BoardPtr->at(NeighborCoord));
                }
            );
        }
    } else if (rIsDown) {
        m_BoardPtr->flag(InteractedCoord);
    }

    Interaction CheckExploded = Interaction::NONE;
    if ((lIsPressed || lIsUp) && (rIsPressed || rIsUp))
        CheckExploded = m_BoardPtr->chording(InteractedCoord);
    else if (lIsUp) {
        if (m_GameHasStarted)
            CheckExploded = m_BoardPtr->open(InteractedCoord);
        else {
            // first click
            m_BoardPtr->adjust(InteractedCoord);
            m_BoardPtr->open(InteractedCoord);
            m_TimerDisplay.run();
            m_GameHasStarted = true;
        }
    }

    if (CheckExploded == Interaction::EXPLODED)
        gameLose();
    else
        checkWinning();
}

void Game::_FaceUpdate(const lan::Vec2<int>& MouseWorldPos) {
    const auto& Mouse = Mouse::getInstance();
    if (MouseWorldPos.x >= m_FacePos.x && MouseWorldPos.x <= m_FacePos.x + m_FaceSize &&
        MouseWorldPos.y >= m_FacePos.y && MouseWorldPos.y <= m_FacePos.y + m_FaceSize) {
        if (Mouse.lIsUp())
            newGame();
    }
}

void Game::_MainGameRender() {
    int offset = 0;
    SDL_Rect Dst = { 0, 0, m_CellSize, m_CellSize };
    auto& Dsts = m_CellTileSet.getDst();
    Dsts.resize(m_BoardPtr->size());
    for (const auto& CurrentCell : *m_BoardPtr) {
        const lan::Vec2<int> CurrentCoord = m_BoardPtr->offsetToCoord(offset);
        Dst.x = m_BoardPos.x + CurrentCoord.x * m_CellSize;
        Dst.y = m_BoardPos.y + CurrentCoord.y * m_CellSize;
        auto& Current = Dsts[offset];
        Current.Rect  = Dst;
        if (CurrentCell.state == CellState::OPENED) {
            if (CurrentCell.hasMine)
                Current.SrcID = static_cast<uint8_t>(TileSet::MINE);
            else
                Current.SrcID = static_cast<uint8_t>(TileSet::ZERO) + CurrentCell.number;
        } else { // CurrentCell.state != CellState::OPENED
            Current.SrcID = static_cast<uint8_t>(CurrentCell.state);
        }
        ++offset;
    }
    RendererPtr->render(m_CellTileSet);
}

void Game::_FaceRender() {
    SDL_Rect Dst = { m_FacePos.x, m_FacePos.y, m_FaceSize, m_FaceSize };
    switch (m_GameHasEnded) {
    case EndingType::NONE:
        RendererPtr->render(m_CellTileSet, m_CellTileSet.getSrc()[static_cast<size_t>(TileSet::FACE_DEFAULT)].Rect, Dst);
        break;

    case EndingType::WIN:
        RendererPtr->render(m_CellTileSet, m_CellTileSet.getSrc()[static_cast<size_t>(TileSet::FACE_WIN)].Rect, Dst);
        break;

    case EndingType::LOSE:
        RendererPtr->render(m_CellTileSet, m_CellTileSet.getSrc()[static_cast<size_t>(TileSet::FACE_LOSE)].Rect, Dst);
        break;
    }
}

} // namespace Minesweeper