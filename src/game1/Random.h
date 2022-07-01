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
	float generateRandom(float min, float max, int seed = std::random_device{}());


}

#endif // !RANDOM_H
