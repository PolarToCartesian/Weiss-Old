#pragma once

#include "../misc/Colors.h"

class ImageLoadingException : public std::exception { };

class Image {
private:
	std::unique_ptr<Coloru8[]> m_buff;

	uint16_t m_width   = 0, m_height = 0;
	uint32_t m_nPixels = 0;

public:
	Image(const Image& img);

	Image(const wchar_t* filename);

	[[nodiscard]] uint16_t GetWidth()                                 const;
	[[nodiscard]] uint16_t GetHeight()                                const;
	[[nodiscard]] uint32_t GetPixelCount()                            const;
	[[nodiscard]] Coloru8* GetBuffer()                                const;
	[[nodiscard]] Coloru8  Sample(const uint16_t x, const uint16_t y) const;

	void Set(const uint16_t x, const uint16_t y, const Coloru8& color);
};