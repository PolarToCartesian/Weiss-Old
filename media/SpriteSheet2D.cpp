#include "SpriteSheet2D.h"

SpriteSheet2D::SpriteSheet2D(const SpriteSheet2DDescriptor& descriptor)
	: Image(WEISS_SPRITE_SHEET_SIDE_LENGTH, WEISS_SPRITE_SHEET_SIDE_LENGTH)
{
	this->m_spriteWidth  = descriptor.sprites[0].second.GetWidth();
	this->m_spriteHeight = descriptor.sprites[0].second.GetHeight();

	uint16_t spriteIndex = 0u;

	for (uint16_t y = 0u; y <= WEISS_SPRITE_SHEET_SIDE_LENGTH - this->m_spriteHeight; y += this->m_spriteHeight)
	{
		for (uint16_t x = 0u; x <= WEISS_SPRITE_SHEET_SIDE_LENGTH - this->m_spriteWidth; x += this->m_spriteWidth)
		{
			if (spriteIndex == descriptor.sprites.size())
				break;

			this->m_spriteLocations.insert({ descriptor.sprites[spriteIndex].first, Vec2u16{ x, y } });

			for (uint16_t x2 = x; x2 < x + this->m_spriteWidth; x2++)
				for (uint16_t y2 = y; y2 < y + this->m_spriteHeight; y2++)
					this->Set(x2, y2, descriptor.sprites[spriteIndex].second.Sample(x2 - x, y2 - y));

			spriteIndex++;
		}
	}
}

[[nodiscard]] uint16_t SpriteSheet2D::GetSpriteWidth() const noexcept
{
	return this->m_spriteWidth;
}

[[nodiscard]] uint16_t SpriteSheet2D::GetSpriteHeight() const noexcept
{
	return this->m_spriteHeight;
}

[[nodiscard]] Vec2u16 SpriteSheet2D::GetSpriteLocation(const std::string& spriteName) const noexcept
{
	return this->m_spriteLocations.at(spriteName);
}