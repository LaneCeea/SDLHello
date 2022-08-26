#ifndef MINESWEEPER_TIMER_H_
#define MINESWEEPER_TIMER_H_

#include <chrono>

namespace Minesweeper {

enum class TimerState {
    NONE = 0, ZERO, RUNNING, PAUSED
};

class Timer {
public:
    using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
    using Duration  = std::chrono::duration<double>;

public:
    Timer() {
        reset();
    }

    inline void reset() {
        m_State = TimerState::ZERO;
        m_Count = 0;
    }

    inline void run() {
        if (m_State == TimerState::RUNNING)
            return;

        m_State = TimerState::RUNNING;
        m_Start = _GlobalNow();
    }

    inline void pause() {
        if (m_State != TimerState::RUNNING)
            return;

        m_State       = TimerState::PAUSED;
        TimePoint End = _GlobalNow();
        Duration Dura = End - m_Start;
        m_Count      += Dura.count();
    }

    inline double now() const {
        switch (m_State) {
        case TimerState::ZERO: {
            return Duration::zero().count();
        }
        case TimerState::RUNNING: {
            TimePoint End = _GlobalNow();
            Duration Dura = End - m_Start;
            return m_Count + Dura.count();
        }
        case TimerState::PAUSED: {
            return m_Count;
        }
        }
        return 0.0;
    }

private:
    static inline TimePoint _GlobalNow() {
        return std::chrono::high_resolution_clock::now();
    }

private:
    TimePoint m_Start;
    double m_Count;
    TimerState m_State;
};

} // namespace Minesweeper

#endif // !MINESWEEPER_TIMER_H_