#include "Image.h"

Image::Image(const char* filename)
{
	std::ifstream file(filename, std::ifstream::binary);

	ASSERT_ERROR(file.is_open(), "Could Not Open Image File");

	// Read Header
	std::string magicNumber;
	float maxColorValue;

	file >> magicNumber >> this->m_width >> this->m_height >> maxColorValue;

	file.ignore(1); // Skip the last whitespace

	this->m_nPixels = this->m_width * this->m_height;
	this->m_buff    = std::make_unique<Vec3u8[]>(this->m_nPixels);

	const uint64_t nSubPixels = static_cast<uint64_t>(this->m_nPixels) * 3;

	ASSERT_ERROR(magicNumber == "P6", "Weiss Only Supports PPM6 Images");

	// Parse Image Data
	file.read((char*)this->m_buff.get(), nSubPixels);

	file.close();
}

uint16_t Image::getWidth()   const { return this->m_width;   }
uint16_t Image::getHeight()  const { return this->m_height;  }
uint32_t Image::getNPixels() const { return this->m_nPixels; }

Vec3u8 Image::sample(const uint16_t x, const uint16_t y) const                { return this->m_buff[y * this->m_width + x]; }
void   Image::set   (const uint16_t x, const uint16_t y, const Vec3u8& color) { this->m_buff[y * this->m_width + x] = color; }

Vec3u8* Image::getBuffer() const { return this->m_buff.get(); }