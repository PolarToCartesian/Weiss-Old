#include "SpriteSheet2D.h"

SpriteSheet2D::SpriteSheet2D(const SpriteSheet2DDescriptor& descriptor)
	: Image(WEISS_SPRITE_SHEET_SIDE_LENGTH, WEISS_SPRITE_SHEET_SIDE_LENGTH)
{
	this->m_spriteWidth  = descriptor.images[0].GetWidth();
	this->m_spriteHeight = descriptor.images[0].GetHeight();

	uint16_t imageIndex = 0u;

	for (uint16_t y = 0u; y <= WEISS_SPRITE_SHEET_SIDE_LENGTH - this->m_spriteHeight; y += this->m_spriteHeight)
	{
		for (uint16_t x = 0u; x <= WEISS_SPRITE_SHEET_SIDE_LENGTH - this->m_spriteWidth; x += this->m_spriteWidth)
		{
			if (imageIndex == descriptor.images.size())
				break;

			for (uint16_t x2 = x; x2 < x + this->m_spriteWidth; x2++)
				for (uint16_t y2 = y; y2 < y + this->m_spriteHeight; y2++)
					this->Set(x2, y2, descriptor.images[imageIndex].Sample(x2 - x, y2 - y));

			imageIndex++;
		}
	}
}