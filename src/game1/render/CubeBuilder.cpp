#include "CubeBuilder.h"

void CubeBuilder::loadVertices(std::uint8_t flags, std::vector<Quad>& quads, const 
	tex::AtlasCollection_t& tex)
{
	for (int i = 0; i < 6; i++)
	{
		if ((flags & (1 << i)))
		{
			quads.push_back(loadFace(tex, static_cast<CubeBuilder::Face>(1 << i)));
		}
	}

}

Cube CubeBuilder::loadVertices(std::uint8_t flags, const tex::AtlasCollection_t& tex)
{
	Cube cube;
	for (int i = 0; i < 6; i++)
	{
		if ((flags & (1 << i)))
		{
			cube.addQuad(loadFace(tex, static_cast<CubeBuilder::Face>(1 << i)));
		}
	}

	return cube;
}

Quad CubeBuilder::loadFace(const tex::AtlasCollection_t& tex, CubeBuilder::Face face)
{
	switch (face)
	{
	case CubeBuilder::Face::BACK:
		//quads.push_back(getBack(tex[0], res));
		return Quad{
			Vertex{ LEFT_BOTTOM_BACK,	0, TEX_BOTTOM_LEFT(tex[0])	},
			Vertex{ RIGHT_TOP_BACK,		0, TEX_TOP_RIGHT(tex[0])	},
			Vertex{ RIGHT_BOTTOM_BACK,	0, TEX_BOTTOM_RIGHT(tex[0])	},
			Vertex{ RIGHT_TOP_BACK,		0, TEX_TOP_RIGHT(tex[0])	},
			Vertex{ LEFT_BOTTOM_BACK,	0, TEX_BOTTOM_LEFT(tex[0])	},
			Vertex{ LEFT_TOP_BACK,		0, TEX_TOP_LEFT(tex[0])		}
		};
		break;
	case CubeBuilder::Face::FRONT:
		return (getFront(tex[1]));
		break;
	case CubeBuilder::Face::LEFT:
		return (getLeft(tex[2]));
		break;
	case CubeBuilder::Face::RIGHT:
		return (getRight(tex[3]));
		break;
	case CubeBuilder::Face::TOP:
		return (getTop(tex[5]));
		break;
	case CubeBuilder::Face::BOTTOM:
		return (getBottom(tex[4]));
		break;
	}
}

Quad CubeBuilder::getBack(const Atlas& atlas)
{
	return Quad{
		Vertex{ LEFT_BOTTOM_BACK,	0, TEX_BOTTOM_LEFT(atlas)	},
		Vertex{ RIGHT_TOP_BACK,		0, TEX_TOP_RIGHT(atlas)		},
		Vertex{ RIGHT_BOTTOM_BACK,	0, TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_BACK,		0, TEX_TOP_RIGHT(atlas)		},
		Vertex{ LEFT_BOTTOM_BACK,	0, TEX_BOTTOM_LEFT(atlas)	},
		Vertex{ LEFT_TOP_BACK,		0, TEX_TOP_LEFT(atlas)		}
	};
}

Quad CubeBuilder::getFront(const Atlas& atlas)
{
	return Quad{
		Vertex{ LEFT_BOTTOM_FRONT,	1,  TEX_BOTTOM_LEFT(atlas)	},
		Vertex{ RIGHT_BOTTOM_FRONT,	1,  TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_FRONT,	1,  TEX_TOP_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_FRONT,	1,  TEX_TOP_RIGHT(atlas)	},
		Vertex{ LEFT_TOP_FRONT,		1,  TEX_TOP_LEFT(atlas)		},
		Vertex{ LEFT_BOTTOM_FRONT,	1,  TEX_BOTTOM_LEFT(atlas)	}
	};
}

Quad CubeBuilder::getLeft(const Atlas& atlas)
{
	return Quad{
		Vertex{ LEFT_TOP_FRONT,		2, TEX_TOP_LEFT	(atlas)		},
		Vertex{ LEFT_TOP_BACK,		2, TEX_TOP_RIGHT(atlas)		},
		Vertex{ LEFT_BOTTOM_BACK,	2, TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ LEFT_BOTTOM_BACK,	2, TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ LEFT_BOTTOM_FRONT,	2, TEX_BOTTOM_LEFT(atlas)	},
		Vertex{ LEFT_TOP_FRONT,		2, TEX_TOP_LEFT(atlas)		}
	};
}

Quad CubeBuilder::getRight(const Atlas& atlas)
{
	return Quad{
		Vertex{ RIGHT_TOP_FRONT,	3, TEX_TOP_LEFT(atlas)		},
		Vertex{ RIGHT_BOTTOM_BACK,	3, TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_BACK,		3, TEX_TOP_RIGHT(atlas)		},
		Vertex{ RIGHT_BOTTOM_BACK,	3, TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_FRONT,	3, TEX_TOP_LEFT(atlas)		},
		Vertex{ RIGHT_BOTTOM_FRONT,	3, TEX_BOTTOM_LEFT(atlas)	}
	};
}

Quad CubeBuilder::getTop(const Atlas& atlas)
{
	return Quad{
		Vertex{ LEFT_TOP_BACK,		4,  TEX_TOP_LEFT(atlas)		},
		Vertex{ RIGHT_TOP_FRONT,	4,  TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_BACK,		4,  TEX_TOP_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_FRONT,	4,  TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ LEFT_TOP_BACK,		4,  TEX_TOP_LEFT(atlas)		},
		Vertex{ LEFT_TOP_FRONT,		4,  TEX_BOTTOM_LEFT(atlas)	}
	};
}

Quad CubeBuilder::getBottom(const Atlas& atlas)
{
	return Quad{
		Vertex{ LEFT_BOTTOM_BACK,	5, TEX_TOP_LEFT(atlas)		},
		Vertex{ RIGHT_BOTTOM_BACK,	5, TEX_TOP_RIGHT(atlas)		},
		Vertex{ RIGHT_BOTTOM_FRONT,	5, TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_BOTTOM_FRONT,	5, TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ LEFT_BOTTOM_FRONT,	5, TEX_BOTTOM_LEFT(atlas)	},
		Vertex{ LEFT_BOTTOM_BACK,	5, TEX_TOP_LEFT(atlas)		}
	};
}

Cube CubeBuilder::loadGrassVertices(const Atlas& atlas)
{
	Cube cube;
	cube.addQuad(Quad{
		Vertex{ LEFT_BOTTOM_FRONT,	-1,  TEX_BOTTOM_LEFT(atlas)		},
		Vertex{ RIGHT_BOTTOM_BACK,	-1,  TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_BACK,		-1,  TEX_TOP_RIGHT(atlas)		},
		Vertex{ RIGHT_TOP_BACK,		-1,  TEX_TOP_RIGHT(atlas)		},
		Vertex{ LEFT_TOP_FRONT,		-1,  TEX_TOP_LEFT(atlas)		},
		Vertex{ LEFT_BOTTOM_FRONT,	-1,  TEX_BOTTOM_LEFT(atlas)		}
		}
	);
	cube.addQuad(Quad{
		Vertex{ LEFT_BOTTOM_BACK,	-1, TEX_BOTTOM_LEFT(atlas)		},
		Vertex{ RIGHT_TOP_FRONT,	-1, TEX_TOP_RIGHT(atlas)		},
		Vertex{ RIGHT_BOTTOM_FRONT,	-1, TEX_BOTTOM_RIGHT(atlas)		},
		Vertex{ RIGHT_TOP_FRONT,	-1, TEX_TOP_RIGHT(atlas)		},
		Vertex{ LEFT_BOTTOM_BACK,	-1, TEX_BOTTOM_LEFT(atlas)		},
		Vertex{ LEFT_TOP_BACK,		-1, TEX_TOP_LEFT(atlas)			}
		}
	);
	return cube;
}
