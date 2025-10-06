#pragma once

#include <stdint.h>
#include <array>

class mt19937 final
{
private:
	static constexpr size_t n = 624;				// State Size
	static constexpr size_t m = 397;				// Shift Size
	static constexpr uint32_t a = 0x9908b0df;		// Twister
	static constexpr uint32_t UMask = 0x80000000;	// Most Significant Bit
	static constexpr uint32_t LMask = 0x7fffffff;	// Least Significant Bit
public:
	mt19937();
	explicit mt19937(const uint32_t& seed);
	~mt19937() = default;

	uint32_t Generate();
	uint32_t GenerateRange(const uint32_t& min, const uint32_t& max);
	float GenerateRange(const float& min, const float& max);
private:
	std::array<uint32_t, n> mt;
	ptrdiff_t index;

	void Twist();
	uint32_t GenerateSeed() const;
};