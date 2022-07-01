#include "CubeBuilder.h"

void CubeBuilder::loadVertices(std::uint8_t flags, std::vector<Quad>& quads, const 
	tex::AtlasCollection_t& tex)
{
	for (int i = 0; i < 6; i++)
	{
		if ((flags & (1 << i)))
		{
			loadFace(quads, tex, static_cast<CubeBuilder::Face>(1 << i) );
		}
	}

}

void CubeBuilder::loadFace(std::vector<Quad>& quads, const tex::AtlasCollection_t& tex,
	CubeBuilder::Face face)
{
	switch (face)
	{
	case CubeBuilder::Face::BACK:
		//quads.push_back(getBack(tex[0], res));
		quads.push_back(Quad{
		Vertex{ LEFT_BOTTOM_BACK,	0, TEX_BOTTOM_LEFT(tex[0])	},
		Vertex{ RIGHT_TOP_BACK,		0, TEX_TOP_RIGHT(tex[0])	},
		Vertex{ RIGHT_BOTTOM_BACK,	0, TEX_BOTTOM_RIGHT(tex[0])	},
		Vertex{ RIGHT_TOP_BACK,		0, TEX_TOP_RIGHT(tex[0])	},
		Vertex{ LEFT_BOTTOM_BACK,	0, TEX_BOTTOM_LEFT(tex[0])	},
		Vertex{ LEFT_TOP_BACK,		0, TEX_TOP_LEFT(tex[0])		}
			}
		);
		break;
	case CubeBuilder::Face::FRONT:
		quads.push_back(getFront(tex[1]));
		break;
	case CubeBuilder::Face::LEFT:
		quads.push_back(getLeft(tex[2]));
		break;
	case CubeBuilder::Face::RIGHT:
		quads.push_back(getRight(tex[3]));
		break;
	case CubeBuilder::Face::TOP:
		quads.push_back(getTop(tex[5]));
		break;
	case CubeBuilder::Face::BOTTOM:
		quads.push_back(getBottom(tex[4]));
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

void CubeBuilder::loadGrassVertices(std::vector<Quad>& quads, const Atlas& atlas)
{
	quads.push_back(Quad{
		Vertex{ LEFT_BOTTOM_FRONT,	-1,  TEX_BOTTOM_LEFT(atlas)		},
		Vertex{ RIGHT_BOTTOM_BACK,	-1,  TEX_BOTTOM_RIGHT(atlas)	},
		Vertex{ RIGHT_TOP_BACK,		-1,  TEX_TOP_RIGHT(atlas)		},
		Vertex{ RIGHT_TOP_BACK,		-1,  TEX_TOP_RIGHT(atlas)		},
		Vertex{ LEFT_TOP_FRONT,		-1,  TEX_TOP_LEFT(atlas)		},
		Vertex{ LEFT_BOTTOM_FRONT,	-1,  TEX_BOTTOM_LEFT(atlas)		}
		}
	);
	quads.push_back(Quad{
		Vertex{ LEFT_BOTTOM_BACK,	-1, TEX_BOTTOM_LEFT(atlas)		},
		Vertex{ RIGHT_TOP_FRONT,	-1, TEX_TOP_RIGHT(atlas)		},
		Vertex{ RIGHT_BOTTOM_FRONT,	-1, TEX_BOTTOM_RIGHT(atlas)		},
		Vertex{ RIGHT_TOP_FRONT,	-1, TEX_TOP_RIGHT(atlas)		},
		Vertex{ LEFT_BOTTOM_BACK,	-1, TEX_BOTTOM_LEFT(atlas)		},
		Vertex{ LEFT_TOP_BACK,		-1, TEX_TOP_LEFT(atlas)			}
		}
	);
}
