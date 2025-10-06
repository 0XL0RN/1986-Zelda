#include "mt19937.h"
#include <random>

mt19937::mt19937()
	: index(n)
{
	mt[0] = GenerateSeed();
	for (int i = 1; i < n; ++i)
		mt[i] = 0x6c078965 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i;
}

mt19937::mt19937(const uint32_t& seed)
	: index(n)
{
	mt[0] = seed;
	for (int i = 1; i < n; ++i)
		mt[i] = 0x6c078965 * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i;
}

uint32_t mt19937::Generate()
{
	if (index >= n)
		Twist();

	uint32_t y = mt[index++];
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680;
	y ^= (y << 15) & 0xefc60000;
	y ^= (y >> 18);
	return y;
}

uint32_t mt19937::GenerateRange(const uint32_t& min, const uint32_t& max)
{
	if (min > max)
		return 0;

	const uint32_t range = max - min + 1;
	const uint32_t value = Generate();
	return min + (value % range);
}

float mt19937::GenerateRange(const float& min, const float& max)
{
	if (min > max)
		return 0.0f;

	const float value = static_cast<float>(Generate()) / (static_cast<uint64_t>(1) << 32);
	return min + value * (max - min);
}

void mt19937::Twist()
{
	for (size_t i = 0; i < n; ++i)
	{
		const uint32_t y = (mt[i] & UMask) | (mt[(i + 1) % n] & LMask);
		mt[i] = mt[(i + m) % n] ^ (y >> 1);
		if (y & 1)
			mt[i] ^= a;
	}
	index = 0;
}

uint32_t mt19937::GenerateSeed() const
{
	std::random_device rd;
	return rd();
}
