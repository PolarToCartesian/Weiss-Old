#pragma once

#include "Vectors.h"

struct Vertex
{
	struct
	{
		float x;
		float y;
		float z;
	} pos;

	float u;
	float v;
	float textureIndex;

	float nx;
	float ny;
	float nz;
};