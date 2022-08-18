#ifndef RNGENGINE_H_
#define RNGENGINE_H_

#include <cstdint>

//
// RNG engines designed by Sebastiano Vigna and David Blackman.
// Their official page: https://prng.di.unimi.it/
//
namespace rng {

// 
// A random number generator with 64-bit initial state. It is based on
// the code from here: https://prng.di.unimi.it/splitmix64.c
//
class SplitMix64 {
public:
	using result_type = std::uint64_t;

public:
	constexpr SplitMix64()
		: m_State(1) {}

	constexpr SplitMix64(std::uint64_t p_Seed)
		: m_State(p_Seed) {}

	constexpr void seed(std::uint64_t p_Seed) {
		m_State = p_Seed;
	}

	constexpr std::uint64_t operator()() {
		std::uint64_t z = (m_State += 0x9e3779b97f4a7c15ull);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		return z ^ (z >> 31);
	}

	static constexpr std::uint64_t(min)() {
		return 0;
	}

	static constexpr std::uint64_t(max)() {
		return UINT64_MAX;
	}

private:
	std::uint64_t m_State; // The state can be seeded with any value.
};

//
// A random number generator with 128-bit initial state. It is based on
// the code from here: https://prng.di.unimi.it/xoroshiro128plus.c
//
class Xoshiro128Plus {
public:
	using result_type = std::uint64_t;

public:
	constexpr Xoshiro128Plus()
		: m_State{ 1, 1 } {}

	constexpr Xoshiro128Plus(std::uint64_t p_Seed0, std::uint64_t p_Seed1)
		: m_State{ p_Seed0, p_Seed1 } {}

	constexpr void seed(std::uint64_t p_Seed0, std::uint64_t p_Seed1) {
		m_State[0] = p_Seed0;
		m_State[1] = p_Seed1;
	}

	static constexpr std::uint64_t rotl(std::uint64_t x, int k) {
		return (x << k) | (x >> (64 - k));
	}

	constexpr std::uint64_t operator()() {
		const std::uint64_t s0     = m_State[0];
		std::uint64_t s1           = m_State[1];
		const std::uint64_t result = s0 + s1;

		s1 ^= s0;
		m_State[0] = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
		m_State[1] = rotl(s1, 37); // c

		return result;
	}

	//
	// This is the jump function for the generator. It is equivalent
	// to 2^64 calls to next(); it can be used to generate 2^64
	// non-overlapping subsequences for parallel computations.
	//
	inline void jump() {
		static constexpr std::uint64_t JUMP[] = { 0xdf900294d8f554a5ull, 0x170865df4b3201fcull };

		std::uint64_t s0 = 0;
		std::uint64_t s1 = 0;
		for (int i = 0; i < sizeof JUMP / sizeof * JUMP; i++)
			for (int b = 0; b < 64; b++) {
				if (JUMP[i] & UINT64_C(1) << b) {
					s0 ^= m_State[0];
					s1 ^= m_State[1];
				}
				(*this)();
			}

		m_State[0] = s0;
		m_State[1] = s1;
	}

	//
	// This is the long-jump function for the generator. It is equivalent to
	// 2^96 calls to next(); it can be used to generate 2^32 starting points,
	// from each of which jump() will generate 2^32 non-overlapping
	// subsequences for parallel distributed computations.
	//
	inline void longJump() {
		static constexpr std::uint64_t LONG_JUMP[] = { 0xd2a98b26625eee7bull, 0xdddf9b1090aa7ac1ull };

		std::uint64_t s0 = 0;
		std::uint64_t s1 = 0;
		for (int i = 0; i < sizeof LONG_JUMP / sizeof * LONG_JUMP; i++)
			for (int b = 0; b < 64; b++) {
				if (LONG_JUMP[i] & UINT64_C(1) << b) {
					s0 ^= m_State[0];
					s1 ^= m_State[1];
				}
				(*this)();
			}

		m_State[0] = s0;
		m_State[1] = s1;
	}

	static constexpr std::uint64_t(min)() {
		return 0;
	}

	static constexpr std::uint64_t(max)() {
		return UINT64_MAX;
	}

private:
	std::uint64_t m_State[2];
};

//
// A 32-bit all-purpose, rock-solid, small-state generator. It is based on
// the code from here: https://prng.di.unimi.it/xoroshiro64starstar.c
//
class Xoroshiro64StarStar {
public:
	using result_type = std::uint32_t;

public:
	constexpr Xoroshiro64StarStar()
		: m_State{ 1, 1 } {}

	constexpr Xoroshiro64StarStar(std::uint32_t p_Seed0, std::uint32_t p_Seed1)
		: m_State{ p_Seed0, p_Seed1 } {}

	constexpr void seed(std::uint32_t p_Seed0, std::uint32_t p_Seed1) {
		m_State[0] = p_Seed0;
		m_State[1] = p_Seed1;
	}

	static constexpr std::uint32_t rotl(std::uint32_t x, int k) {
		return (x << k) | (x >> (32 - k));
	}

	constexpr std::uint32_t operator()() {
		const uint32_t s0     = m_State[0];
		uint32_t s1           = m_State[1];
		const uint32_t result = rotl(s0 * 0x9E3779BB, 5) * 5;

		s1 ^= s0;
		m_State[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9); // a, b
		m_State[1] = rotl(s1, 13); // c

		return result;
	}

	static constexpr std::uint32_t(min)() {
		return 0;
	}

	static constexpr std::uint32_t(max)() {
		return UINT32_MAX;
	}

private:
	std::uint32_t m_State[2];
};

} // namespace rng

#endif // !RNGENGINE_H_