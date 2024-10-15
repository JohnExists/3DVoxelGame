#ifndef RANDOM_H
#define RANDOM_H

#include<random>
#include<string>
#include<cmath>
#include<vector>
#include<map>
#include<glm/glm.hpp>

namespace random
{
	/**
	 * 
	 * @param min The minimum value the RNG can produce
	 * @param max The maximum value the RNG can produce
	 * @param seed The integer value that determines how "random" numbers are
	 * @returns 
	 */
	float generateRandom(float min, float max, int seed = std::random_device{}());

}

#endif // !RANDOM_H
