#include"Decoration.h"

decor::positions_t decor::loadPostions(std::string&& filePath) 
{
	return loadFromFile("../res/" + filePath);
}

decor::positions_t decor::load(Decoration decoration)
{
	static decor::positions_t lookupDecoration[] = {
		loadPostions("Tree.pos"),
		loadPostions("GrassBlade.pos"),
	};

	int decorationIndex = static_cast<int>(decoration);
	return lookupDecoration[decorationIndex];
}

decor::positions_t decor::loadFromFile(std::string filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) throw("Failed to open file");

	decor::positions_t blockPositions;

	while (!file.eof()) // While not end of file
	{
		char line[128];
		char delimiter = '|';

		file.getline(line, 128);
		std::string lineAsString(line);

		size_t pos = 0;
		int counter = 0;
		int tokens[4] { };
		std::string token;
		while (((pos = lineAsString.find(delimiter)) != std::string::npos) &&
			lineAsString.size() != 0) {
			// converts the string token to an int
			tokens[counter] = std::stoi(lineAsString.substr(0, pos));

			lineAsString.erase(0, pos + 1);
			counter++;
		}


		if (tokens[1] == 0 && tokens[2] == 0 && tokens[3] == 0) continue;

		blockPositions.insert(std::make_pair(
			static_cast<BlockType>(tokens[0]), glm::vec3(tokens[1], tokens[2], tokens[3])
		));
	}

	return blockPositions;
}
