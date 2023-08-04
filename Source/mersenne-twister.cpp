#include <stdio.h>
#include <stdint.h>
#include "mersenne-twister.h"

#ifndef UINT32_MAX
#define UINT32_MAX 4294967295U
#endif

static const unsigned SIZE = 624;
static const unsigned PERIOD = 397;
static const unsigned DIFF = SIZE - PERIOD;

static uint32_t MT[SIZE];
static unsigned index = 0;

#define M32(x) (0x80000000 & x) // 32nd Most Significant Bit
#define L31(x) (0x7FFFFFFF & x) // 31 Least Significant Bits
#define ODD(x) (x & 1) // Check if number is odd

namespace mt
{

	static inline void generate_numbers()
	{
		static const uint32_t MATRIX[2] = { 0, 0x9908b0df };
		register uint32_t y, i;

		for (i = 0; i < DIFF; ++i)
		{
			y = M32(MT[i]) | L31(MT[i + 1]);
			MT[i] = MT[i + PERIOD] ^ (y >> 1) ^ MATRIX[ODD(y)];

			++i;
			y = M32(MT[i]) | L31(MT[i + 1]);
			MT[i] = MT[i + PERIOD] ^ (y >> 1) ^ MATRIX[ODD(y)];
		}

#define UNROLL \
    y = M32(MT[i]) | L31(MT[i+1]); \
    MT[i] = MT[i-DIFF] ^ (y>>1) ^ MATRIX[ODD(y)]; \
    ++i;

		for (i = DIFF; i < (SIZE - 1); )
		{
			// 11 times
			UNROLL; UNROLL; UNROLL;
			UNROLL; UNROLL; UNROLL;

			UNROLL; UNROLL; UNROLL;
			UNROLL; UNROLL;
		}

		y = M32(MT[SIZE - 1]) | L31(MT[SIZE]);
		MT[SIZE - 1] = MT[PERIOD - 1] ^ (y >> 1) ^ MATRIX[ODD(y)];
	}

	void seed(uint32_t value)
	{
		MT[0] = value;

		for (register unsigned i = 1; i < SIZE; ++i)
			MT[i] = 0x6c078965 * (MT[i - 1] ^ MT[i - 1] >> 30) + i;
	}

	uint32_t rand_u32()
	{
		if (!index)
			generate_numbers();

		register uint32_t y = MT[index];

		// Tempering
		y ^= y >> 11;
		y ^= y << 7 & 0x9d2c5680;
		y ^= y << 15 & 0xefc60000;
		y ^= y >> 18;

		if (++index == SIZE)
			index = 0;

		return y;
	}

	int rand()
	{
		return static_cast<int>(0x7FFFFFFF & rand_u32());
	}

	void srand(unsigned value)
	{
		seed(static_cast<uint32_t>(value));
	}

	float randf_cc()
	{
		return static_cast<float>(rand_u32()) / UINT32_MAX;
	}

	float randf_co()
	{
		return static_cast<float>(rand_u32()) / (UINT32_MAX + 1.0f);
	}

	float randf_oo()
	{
		return (static_cast<float>(rand_u32()) + 0.5f) / (UINT32_MAX + 1.0f);
	}

	uint64_t rand_u64()
	{
		return static_cast<uint64_t>(rand_u32()) << 32 | rand_u32();
	}
}