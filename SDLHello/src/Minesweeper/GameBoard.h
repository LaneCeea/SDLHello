#ifndef MINESWEEPER_GAMEBOARD_H_
#define MINESWEEPER_GAMEBOARD_H_

#include <cstdlib>
#include <array>
#include <vector>
#include <functional>
#include <Vec/Vec2.h>

#include "SDLErrorHandler.h"

namespace Minesweeper {

enum class CellState {
    CLOSED = 0,
    FLAGED,
    WRONGFLAGED,
    PRESSED,
    OPENED,
    EXPLODED = OPENED + 10
};

struct Cell {
    int number;
    bool hasMine;
    CellState state;

    static constexpr int MINE = 9; // for rendering purpose

    Cell() : number(0), hasMine(false), state(CellState::CLOSED) {}
};

enum class Interaction {
    NONE = 0,
    FAILED,
    SUCCESS,
    EXPLODED
};

class GameBoard {
public:
    using ContainerType = std::vector<Cell>;

public:
    GameBoard(int rows, int columns, int mines);

    void adjust(const lan::Vec2<int32_t>& FirstClick);

private:
    void _GenerateMine();

    // call cascade on a opened cell is allowed
    void _Cascade(const lan::Vec2<int32_t>& Coord);

public:
    void forEachNeighbor(const lan::Vec2<int32_t> CenterCoord,
        std::function<void(Cell&)> Function);

    void forEachNeighbor(const lan::Vec2<int32_t> CenterCoord,
        std::function<void(const lan::Vec2<int32_t>&)> Function);

public:

    ///////////////////////////////////////////////////////////////////////////
    // Player Interaction
    ///////////////////////////////////////////////////////////////////////////

    inline Interaction open(const lan::Vec2<int32_t>& InteractedCoord) {
        auto& CurrentCell = at(InteractedCoord);
        auto& State = CurrentCell.state;
        if (State == CellState::CLOSED) {
            DEBUGLOG("[Game Interaction] open\n");
            if (CurrentCell.hasMine) {
                CurrentCell.state = CellState::EXPLODED;
                return Interaction::EXPLODED;
            }
            _Cascade(InteractedCoord);
            return Interaction::SUCCESS;
        }
        return Interaction::FAILED;
    }

    inline Interaction press(const lan::Vec2<int32_t>& InteractedCoord) {
        auto& State = at(InteractedCoord).state;
        if (State == CellState::CLOSED) {
            State = CellState::PRESSED;
            return Interaction::SUCCESS;
        }
        return Interaction::FAILED;
    }

    inline Interaction flag(const lan::Vec2<int32_t>& InteractedCoord) {
        auto& State = at(InteractedCoord).state;
        if (State == CellState::CLOSED) {
            DEBUGLOG("[Game Interaction] flag\n");
            State = CellState::FLAGED;  // flag
            return Interaction::SUCCESS;
        } else if (State == CellState::FLAGED) {
            DEBUGLOG("[Game Interaction] unflag\n");
            State = CellState::CLOSED;  // unflag
            return Interaction::FAILED;
        }
        return Interaction::NONE;
    }

    inline Interaction chording(const lan::Vec2<int32_t>& InteractedCoord) {
        auto& CurrentCell = at(InteractedCoord);
        if (CurrentCell.state == CellState::OPENED) {
            int flag_count = 0;
            forEachNeighbor(InteractedCoord,
                [&flag_count](const Cell& NeighborCell) {
                    if (NeighborCell.state == CellState::FLAGED)
                        ++flag_count;
                }
            );
            if (flag_count == CurrentCell.number) {
                DEBUGLOG("[Game Interaction] chording\n");
                bool exploded = false;
                forEachNeighbor(InteractedCoord,
                    [this, &exploded](const lan::Vec2<int32_t>& NeighborCoord) {
                        if (open(NeighborCoord) == Interaction::EXPLODED)
                            exploded = true;
                    }
                );
                if (exploded)
                    return Interaction::EXPLODED;
                return Interaction::SUCCESS;
            }
        }
        return Interaction::FAILED;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Dimension Convert
    ///////////////////////////////////////////////////////////////////////////

    template <typename Ty>
    inline lan::Vec2<Ty> offsetToCoord(Ty offset) const {
        const std::div_t Coord = std::div(offset, COLUMNS);
        return lan::Vec2<Ty>(Coord.rem, Coord.quot);
    }

    template <typename Ty>
    constexpr Ty coordToOffset(const lan::Vec2<Ty> Coord) const {
        return Coord.y * COLUMNS + Coord.x;
    }

    template <typename Ty>
    inline bool isInRange(const lan::Vec2<Ty>& Coord) const {
        if (Coord.x >= 0 && Coord.x < COLUMNS &&
            Coord.y >= 0 && Coord.y < ROWS)
            return true;
        return false;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Element Access 
    ///////////////////////////////////////////////////////////////////////////

    inline size_t size() const {
        return m_Cells.size();
    }

    inline Cell& at(size_t offset) {
        return m_Cells[offset];
    }

    inline const Cell& at(size_t offset) const {
        return m_Cells[offset];
    }

    inline Cell& at(const lan::Vec2<int> vec) {
        return at(coordToOffset(vec));
    }

    inline const Cell& at(const lan::Vec2<int> vec) const {
        return at(coordToOffset(vec));
    }

    ///////////////////////////////////////////////////////////////////////////
    // Iterator
    ///////////////////////////////////////////////////////////////////////////

    inline ContainerType::iterator begin() {
        return m_Cells.begin();
    }

    inline ContainerType::const_iterator begin() const {
        return m_Cells.begin();
    }

    inline ContainerType::const_iterator cbegin() const {
        return m_Cells.cbegin();
    }

    inline ContainerType::iterator end() {
        return m_Cells.end();
    }

    inline ContainerType::const_iterator end() const {
        return m_Cells.end();
    }

    inline ContainerType::const_iterator cend() const {
        return m_Cells.cend();
    }

public:
    static const std::array<lan::Vec2<int32_t>, 8> Translates;
    const int ROWS;
    const int COLUMNS;
    const int MINES;

private:
    ContainerType m_Cells;
};

} // namespace Minesweeper

#endif // !MINESWEEPER_GAMEBOARD_H_