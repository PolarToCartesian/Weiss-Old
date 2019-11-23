#pragma once

#include "../../misc/Error.h"
#include "../../../math/Vectors.h"

#include <array>
#include <memory>
#include <fstream>
#include <sstream>

class Image {
	private:
		std::unique_ptr<Vec3u8[]> m_buff;

		uint16_t m_width = 0, m_height = 0;
		uint32_t m_nPixels = 0;

	public:
		Image(const char* filename);

		uint16_t getWidth()   const;
		uint16_t getHeight()  const;
		uint32_t getNPixels() const;

		Vec3u8 sample(const uint16_t x, const uint16_t y) const;
		void   set   (const uint16_t x, const uint16_t y, const Vec3u8& color);

		Vec3u8* getBuffer() const;
};