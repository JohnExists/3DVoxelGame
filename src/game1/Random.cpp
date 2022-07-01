#include "Random.h"
#include <iostream>


float random::generateRandom(float min, float max, int seed)
{
	static std::mt19937 mt(seed);
	std::uniform_real_distribution<float> randOutput(min, max);

	return randOutput(mt);
}