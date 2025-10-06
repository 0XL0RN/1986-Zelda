#pragma once

#include <cstdint>

/////////////////////////////////////////////////////
// Romaan's Simple XORSHIFT
/////////////////////////////////////////////////////
class Xorshift
{
private:
	uint32_t m_seed;
public:
	Xorshift();
	uint32_t GetRNG(int min, int max);
private:
	uint32_t GenSeed();
};
