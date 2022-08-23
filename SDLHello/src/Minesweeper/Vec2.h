#ifndef MINESWEEPER_VEC2_H_
#define MINESWEEPER_VEC2_H_

namespace Minesweeper {

template <typename Ty>
struct Vec2 {
    Ty x, y;
    
    Vec2() : x(0), y(0) {}
    Vec2(Ty px, Ty py) : x(px), y(py) {}

    template <typename OtherTy>
    Vec2(const Vec2<OtherTy> Right) : x(static_cast<Ty>(Right.x)), y(static_cast<Ty>(Right.y)) {}

    template <typename OtherTy>
    Vec2& operator=(const Vec2<OtherTy> Right) {
        x = static_cast<Ty>(Right.x);
        y = static_cast<Ty>(Right.y);
    }

    Vec2& operator+=(const Vec2& Right) {
        x += Right.x;
        y += Right.y;
        return *this;
    }

    Vec2& operator-=(const Vec2& Right) {
        x -= Right.x;
        y -= Right.y;
        return *this;
    }

    Vec2& operator*=(Ty scaler) {
        x *= scaler;
        y *= scaler;
        return *this;
    }

    Vec2& operator/=(Ty scaler) {
        x /= scaler;
        y /= scaler;
        return *this;
    }
};

template <typename Ty>
Vec2<Ty> operator+(const Vec2<Ty> Left, const Vec2<Ty> Right) {
    Vec2<Ty> Result(Left);
    Result += Right;
    return Result;
}

template <typename Ty>
Vec2<Ty> operator-(const Vec2<Ty> Left, const Vec2<Ty> Right) {
    Vec2<Ty> Result(Left);
    Result -= Right;
    return Result;
}

template <typename Ty>
Vec2<Ty> operator*(const Vec2<Ty> Left, Ty Right) {
    Vec2<Ty> Result(Left);
    Result *= Right;
    return Result;
}

template <typename Ty>
Vec2<Ty> operator/(const Vec2<Ty> Left, Ty Right) {
    Vec2<Ty> Result(Left);
    Result /= Right;
    return Result;
}


} // namespace Minesweeper

#endif // !MINESWEEPER_VEC2_H_