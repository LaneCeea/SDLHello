#ifndef RNG_H_
#define RNG_H_

#include <random>
#include <type_traits>
#include <vector>

#include "RngEngine.h"

namespace lan {

// singleton 64-bit rng class
template <typename Ty>
class RandomInt64 {
public:
    using result_type = Ty;

public:
    static RandomInt64& getInstance() {
        static RandomInt64 s_Instance;
        return s_Instance;
    }

    void setParam(Ty lower, Ty upper) {
        m_Dist.param(std::uniform_int_distribution<Ty>::param_type(lower, upper));
    }

    result_type getNum() {
        return m_Dist(m_Engine);
    }

    result_type getNum(Ty lower, Ty upper) {
        return m_Dist(m_Engine, std::uniform_int_distribution<Ty>::param_type(lower, upper));
    }

    void jump() {
        m_Engine.jump();
    }

    void longJump() {
        m_Engine.longJump();
    }

    RandomInt64(const RandomInt64&)            = delete;
    RandomInt64& operator=(const RandomInt64&) = delete;

private:
    RandomInt64() : m_Seed(), m_InterSeed(), m_Engine(), m_Dist() {
        std::uint64_t val = m_Seed();
        val = (val << 32) | m_Seed();
        m_InterSeed.seed(val);
        m_Engine.seed(m_InterSeed(), m_InterSeed());
    }

private:
    std::random_device m_Seed;
    rng::SplitMix64 m_InterSeed;
    rng::Xoshiro128Plus m_Engine;
    std::uniform_int_distribution<Ty> m_Dist;
};

// singleton 32-bit rng class
template <typename Ty>
class RandomInt32 {
public:
    using result_type = Ty;

public:
    static RandomInt32& getInstance() {
        static RandomInt32 s_Instance;
        return s_Instance;
    }

    void setParam(Ty lower, Ty upper) {
        m_Dist.param(std::uniform_int_distribution<Ty>::param_type(lower, upper));
    }

    result_type getNum() {
        return m_Dist(m_Engine);
    }

    result_type getNum(Ty lower, Ty upper) {
        return m_Dist(m_Engine, std::uniform_int_distribution<Ty>::param_type(lower, upper));
    }

    RandomInt32(const RandomInt32&)            = delete;
    RandomInt32& operator=(const RandomInt32&) = delete;

private:
    RandomInt32() : m_Seed(), m_Engine(), m_Dist() {
        m_Engine.seed(m_Seed(), m_Seed());
    }

private:
    std::random_device m_Seed;
    rng::Xoroshiro64StarStar m_Engine;
    std::uniform_int_distribution<Ty> m_Dist;
};

template <typename Ty>
constexpr bool is64Bit = (std::is_integral_v<Ty> && sizeof(Ty) == 8);

template <typename Ty, class Rng = std::conditional_t<is64Bit<Ty>, RandomInt64<Ty>, RandomInt32<Ty>>>
Rng& getRngInstance() {
    return Rng::getInstance();
}

// This function generates a total of `count` unduplicated numbers
// ranging from [`lower`, `upper`]. The sequence is stored in `Result`.
// cons: allocate at least ((upper - lower + 1) * sizeof(Ty)) bytes of memory
template <typename Ty>
std::vector<Ty>& randomSequence(std::vector<Ty>& Result, Ty lower, Ty upper, Ty count) {
    Ty interval = upper - lower + 1;

    std::vector<Ty> Available;
    Available.reserve(interval);
    for (Ty val = lower; val < upper; ++val)
        Available.push_back(val);
    Available.push_back(upper); // prevent val from overflowing

    auto& Rand = getRngInstance<Ty>();
    Ty last = interval - 1;
    // shuffle [0, count)
    for (Ty target = 0; target < count; ++target) {
        Ty offset = Rand.getNum(target, last);
        if (target != offset)
            std::swap(Available[target], Available[offset]);
    }

    Result.assign(Available.cbegin(), Available.cbegin() + count);
    return Result;
}

} // namespace lan

#endif // !RNG_H_