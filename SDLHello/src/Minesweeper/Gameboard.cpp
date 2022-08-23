#include "GameBoard.h"

#include <iostream>
#include <Rng/Rng.h>

namespace Minesweeper {

const std::array<Vec2<int32_t>, 8> GameBoard::Translates = {
    Vec2<int32_t>(-1,  1), Vec2<int32_t>(0,  1), Vec2<int32_t>(1,  1),
    Vec2<int32_t>(-1,  0)                      , Vec2<int32_t>(1,  0),
    Vec2<int32_t>(-1, -1), Vec2<int32_t>(0, -1), Vec2<int32_t>(1, -1)
};

GameBoard::GameBoard(int rows, int columns, int mines)
    : ROWS(rows), COLUMNS(columns), MINES(mines),
    m_Cells(static_cast<size_t>(ROWS) * COLUMNS, Cell()) {
    DEBUGLOG("GameBoard's constructor called!\n");
    _GenerateMine();
}

void GameBoard::adjust(const Vec2<int32_t>& FirstClick) {
    auto& CurrentCell = at(FirstClick);
    if (CurrentCell.hasMine) {
        DEBUGLOG("Adjusting the mine...");
        // fill the number
        CurrentCell.hasMine = false;
        CurrentCell.number = 0;
        _ForEachNeighbor(FirstClick,
            [&CurrentCell](Cell& NeighborCell) {
                --NeighborCell.number;
                if (NeighborCell.hasMine)
                    ++CurrentCell.number;
            }
        );

        // relocate the mine
        int32_t offset = 0;
        for (auto& ResultCell : m_Cells) {
            if (!ResultCell.hasMine) {
                ResultCell.hasMine = true;
                _ForEachNeighbor(offsetToCoord(offset),
                    [](Cell& NeighborCell) {
                        ++NeighborCell.number;
                    }
                );
                break;
            }
            ++offset;
        }
        DEBUGLOG("Success!\n");
    }
}

void GameBoard::_GenerateMine() {
    DEBUGLOG("Generating mines...");
    std::vector<int32_t> MinesOffset;
    lan::randomSequence<int32_t>(MinesOffset, 0, (int32_t)size() - 1, MINES);

    for (const auto& offset : MinesOffset) {
        m_Cells[offset].hasMine = true;
        _ForEachNeighbor(offsetToCoord(offset),
            [](Cell& NeighborCell) {
                ++NeighborCell.number;
            }
        );
    }
    DEBUGLOG("Success!\n");
}

void GameBoard::_Cascade(const Vec2<int32_t>& Coord) {
    if (!isInRange(Coord))
        return;

    auto& CurrentCell = at(Coord);

    if (CurrentCell.state == CellState::FLAGED)
        return;

    CurrentCell.state = CellState::OPENED;

    if (CurrentCell.number != 0)
        return;

    _ForEachNeighbor(Coord, 
        [this](const Vec2<int32_t>& NeighborCoord) {
            if (at(NeighborCoord).state == CellState::CLOSED)
                _Cascade(NeighborCoord);
        }
    );

}

void GameBoard::_ForEachNeighbor(const Vec2<int32_t> CenterCoord, std::function<void(Cell&)> Function) {
    for (const auto& Translate : Translates) {
        Vec2<int32_t> NeighborCoord = CenterCoord + Translate;
        if (isInRange(NeighborCoord))
            Function(at(NeighborCoord));
    }
}

void GameBoard::_ForEachNeighbor(const Vec2<int32_t> CenterCoord, std::function<void(const Vec2<int32_t>&)> Function) {
    for (const auto& Translate : Translates) {
        Vec2<int32_t> NeighborCoord = CenterCoord + Translate;
        if (isInRange(NeighborCoord))
            Function(NeighborCoord);
    }
}

} // namespace Minesweeper