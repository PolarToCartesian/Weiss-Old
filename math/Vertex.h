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
	
	struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	} color;
};