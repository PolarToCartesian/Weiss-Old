#pragma once

#include "../math/Vectors.h"

struct Coloru8
{
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};

class Image {
	private:
		std::unique_ptr<Coloru8[]> m_buff;

		uint16_t m_width = 0, m_height = 0;
		uint32_t m_nPixels = 0;

	public:
		Image(const wchar_t* filename);

		uint16_t getWidth()   const;
		uint16_t getHeight()  const;
		uint32_t getNPixels() const;

		Coloru8 sample(const uint16_t x, const uint16_t y) const;
		void    set   (const uint16_t x, const uint16_t y, const Coloru8& color);

		Coloru8* getBuffer() const;
};