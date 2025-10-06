#include "Xorshift.h"
#include <random>

Xorshift::Xorshift()
	: m_seed{ 0 }
{
}

/////////////////////////////////////////////////////
// Outputting RNG based on min/max
/////////////////////////////////////////////////////
uint32_t Xorshift::GetRNG(int min, int max)
{
	uint32_t rngState = GenSeed();
	uint32_t num = rngState;
	num ^= num << 13;
	num ^= num >> 17;
	num ^= num << 5;
	rngState = num;

	return (m_seed) % (max - min + 1) + min;
}

/////////////////////////////////////////////////////
//  Generates seed
/////////////////////////////////////////////////////
uint32_t Xorshift::GenSeed()
{
	std::random_device rd;
	m_seed = rd();
	return m_seed;
}

