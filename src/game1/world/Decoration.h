#ifndef DECORATION_H
#define DECORATION_H

#include<map>
#include<glm/glm.hpp>
#include<fstream>
#include<string>

#include"Block.h"
#include"../Constants.h"

namespace decor
{

	using positions_t = std::multimap<BlockType, glm::vec3>;

	positions_t* loadTree();
	positions_t loadPostions(std::string&& filePath);
	decor::positions_t load(Decoration decoration);
	decor::positions_t loadFromFile(std::string filePath);

}

#endif // !DECORATION_H


