#include "Biome.h"

//////////////////////////////////
/*		Public Functions		*/
//////////////////////////////////

Biome::Biome(int scaleBy, int addBy, BlockType top, BlockType center, BlockType bottom, 
	std::initializer_list<Placeables_t> decoration) : 
	scaleBy(scaleBy), addBy(addBy), 
	topBlock(top), centerBlocks(center), bottomBlocks(bottom)
{
	this->decorationCount = decoration.size();
	if(decorationCount == 0) return;

	this->decoration = new Placeables_t[decorationCount];
	int i = 0;

	for (auto&& decor : decoration)
	{
		this->decoration[i] = decor;
		i++;
	}
	std::sort(this->decoration, this->decoration + decorationCount);
}


BlockType Biome::get(int valueToGet)
{
	switch (valueToGet)
	{
	case TOP: 		return topBlock;
	case CENTER: 	return centerBlocks;
	case BOTTOM: 	return bottomBlocks;
	default: 		return BlockType::AIR;
	}
}

int Biome::obtainHeight(float noiseOutput)
{
	return (noiseOutput * scaleBy + addBy);
}

Decoration Biome::obtainDecoration()
{
	std::vector<Placeables_t> place;
	for (int i = 0; i < decorationCount; i++)
	{
		if(shouldPlace(decoration[i].first))
		{
			place.push_back(decoration[i]);
		}
	}
	if(place.empty()) return Decoration::NOTHING;
	return place.at(0).second;
}

bool operator<(const Biome::Placeables_t& left, const Biome::Placeables_t& right)
{
	return left.first < right.first;
}

bool Biome::shouldPlace(float percentage)
{
	return (percentage / 100.0f) > (random::generateRandom(0, 1));
}


