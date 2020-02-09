#pragma once

#include "Texture2D.h"
#include "../misc/Defines.h"

class SpriteSheet2DCreationException : public std::exception { };

struct SpriteSheet2DDescriptor
{
	std::vector<Image> images;
};

class SpriteSheet2D : public Image {
private:
	uint16_t m_spriteWidth = 0u, m_spriteHeight = 0u;

public:
	SpriteSheet2D(const SpriteSheet2DDescriptor& descriptor);
};