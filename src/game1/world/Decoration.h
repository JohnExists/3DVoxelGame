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

	// List of Block and the position of that block
	using positions_t = std::multimap<BlockType, glm::vec3>;

	/**
	 * Generates the positions and block data from a specific .pos file in the /res folder
	 * 
	 * @param filePath The file location of the .pos for the decoration structure
	 * @returns Returns a list of locations and blocks (3d vector and block type)
	 */
	positions_t loadPostions(std::string&& filePath);

	/**
	 * Loads a specific decorations blocktype and locations
	 * 
	 * @param decoration The decoration that will be placed down (ex. TREE)
	 * @return The list of 3d vectors and block type pair for that specific
	 * decoration
	 */
	decor::positions_t load(Decoration decoration);

	/**
	 * Generates the positions and block data from a specific .pos file
	 * 
	 * @param filePath The exact .pos file location relative to computer home
	 * @return Returns a list of locations and blocks (3d vector and block type)
	 */
	decor::positions_t loadFromFile(std::string filePath);

}

#endif // !DECORATION_H


