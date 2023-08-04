#ifndef MERSENNE_TWISTER_H
#define MERSENNE_TWISTER_H

#include <stdint.h>

namespace mt
{
	#define RAND_MAX INT16_MAX

	void srand(unsigned seed_value);
	int rand();
	uint16_t rand_u16();
	void seed(uint16_t seed_value);
	float randf_cc();
	float randf_co();
	float randf_oo();
}
#endif