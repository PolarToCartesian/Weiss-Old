#pragma once

#include "Texture2D.h"
#include "../math/Vectors.h"
#include "../misc/Defines.h"

class SpriteSheet2DCreationException : public std::exception { };

struct SpriteSheet2DDescriptor
{
	std::vector<std::pair<std::string, Image>> sprites;
};

class SpriteSheet2D : public Image {
private:
	uint16_t m_spriteWidth = 0u, m_spriteHeight = 0u;

	std::unordered_map<std::string, Vec2u16> m_spriteLocations;

public:
	SpriteSheet2D(const SpriteSheet2DDescriptor& descriptor);

	[[nodiscard]] uint16_t GetSpriteWidth()  const noexcept;
	[[nodiscard]] uint16_t GetSpriteHeight() const noexcept;
	[[nodiscard]] Vec2u16  GetSpriteLocation(const std::string& spriteName) const noexcept;
};