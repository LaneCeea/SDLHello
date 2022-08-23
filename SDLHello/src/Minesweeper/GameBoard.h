#ifndef MINESWEEPER_GAMEBOARD_H_
#define MINESWEEPER_GAMEBOARD_H_

#include <cstdlib>
#include <array>
#include <vector>
#include <functional>

#include "SDLErrorHandler.h"
#include "Vec2.h"

namespace Minesweeper {

enum class CellState {
    CLOSED = 0,
    FLAGED,
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

    void adjust(const Vec2<int32_t>& FirstClick);

private:
    void _GenerateMine();

    // call cascade on a opened cell is allowed
    void _Cascade(const Vec2<int32_t>& Coord);

    void _ForEachNeighbor(const Vec2<int32_t> CenterCoord,
        std::function<void(Cell&)> Function);

    void _ForEachNeighbor(const Vec2<int32_t> CenterCoord,
        std::function<void(const Vec2<int32_t>&)> Function);

public:

    ///////////////////////////////////////////////////////////////////////////
    // Player Interaction
    ///////////////////////////////////////////////////////////////////////////

    inline Interaction open(const Vec2<int32_t>& InteractedCoord) {
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

    inline Interaction press(const Vec2<int32_t>& InteractedCoord) {
        auto& State = at(InteractedCoord).state;
        if (State == CellState::CLOSED) {
            State = CellState::PRESSED;
            return Interaction::SUCCESS;
        }
        return Interaction::FAILED;
    }

    inline Interaction flag(const Vec2<int32_t>& InteractedCoord) {
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
    }

    inline Interaction chording(const Vec2<int32_t>& InteractedCoord) {
        int flag_count = 0;
        _ForEachNeighbor(InteractedCoord,
            [&flag_count](const Cell& NeighborCell) {
                if (NeighborCell.state == CellState::FLAGED)
                    ++flag_count;
            }
        );
        if (flag_count == at(InteractedCoord).number) {
            DEBUGLOG("[Game Interaction] chording\n");
            bool exploded = false;
            _ForEachNeighbor(InteractedCoord,
                [this, &exploded](const Vec2<int32_t>& NeighborCoord) {
                    if (open(NeighborCoord) == Interaction::EXPLODED)
                        exploded = true;
                }
            );
            if (exploded)
                return Interaction::EXPLODED;
            return Interaction::SUCCESS;
        }
        return Interaction::FAILED;
    }


    ///////////////////////////////////////////////////////////////////////////
    // Dimension Convert
    ///////////////////////////////////////////////////////////////////////////

    template <typename Ty>
    inline Vec2<Ty> offsetToCoord(Ty offset) const {
        const std::div_t Coord = std::div(offset, COLUMNS);
        return Vec2<Ty>(Coord.rem, Coord.quot);
    }

    template <typename Ty>
    constexpr Ty coordToOffset(const Vec2<Ty> Coord) const {
        return Coord.y * COLUMNS + Coord.x;
    }

    template <typename Ty>
    inline bool isInRange(const Vec2<Ty>& Coord) const {
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

    inline Cell& at(const Vec2<int> vec) {
        return at(coordToOffset(vec));
    }

    inline const Cell& at(const Vec2<int> vec) const {
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
    const int ROWS;
    const int COLUMNS;
    const int MINES;

private:
    ContainerType m_Cells;

private:
    static const std::array<Vec2<int32_t>, 8> Translates;
};

} // namespace Minesweeper

#endif // !MINESWEEPER_GAMEBOARD_H_