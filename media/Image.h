#pragma once

#include "../misc/Colors.h"

class ImageLoadingException : public std::exception { };

class Image {
private:
	std::unique_ptr<Coloru8[]> m_buff;

	uint16_t m_width   = 0, m_height = 0;
	uint32_t m_nPixels = 0;

public:
	Image(const wchar_t* filename)
	{
		Microsoft::WRL::ComPtr<IWICBitmapSource>      decodedConvertedFrame;
		Microsoft::WRL::ComPtr<IWICBitmapDecoder>     bitmapDecoder;
		Microsoft::WRL::ComPtr<IWICImagingFactory>    factory;
		Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frameDecoder;
		
		if (CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory)) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create IWICImagingFactory")
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		if (factory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &bitmapDecoder) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Read/Open Image");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		if (bitmapDecoder->GetFrame(0, &frameDecoder) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Get First Frame Of Image");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		if (frameDecoder->GetSize((UINT*)&this->m_width, (UINT*)&this->m_height) != S_OK)
		{

#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Get Image Width/Height");
#endif // __WEISS_SHOW_DEBUG_ERRORS
			
			throw ImageLoadingException();
		}

		this->m_nPixels = this->m_width * this->m_height;

		if (WICConvertBitmapSource(GUID_WICPixelFormat32bppRGBA, frameDecoder.Get(), &decodedConvertedFrame) != S_OK)
		{

#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Bitmap Converter");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}

		this->m_buff = std::make_unique<Coloru8[]>(this->m_nPixels * sizeof(Coloru8));

		const WICRect sampleRect{ 0, 0, this->m_width, this->m_height };

		if (decodedConvertedFrame->CopyPixels(&sampleRect, this->m_width * sizeof(Coloru8), this->m_nPixels * sizeof(Coloru8), (BYTE*)this->m_buff.get()) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Copy Pixels From Bitmap");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw ImageLoadingException();
		}
	}

	[[nodiscard]] uint16_t GetWidth()                                 const { return this->m_width;                       }
	[[nodiscard]] uint16_t GetHeight()                                const { return this->m_height;                      }
	[[nodiscard]] uint32_t GetPixelCount()                            const { return this->m_nPixels;                     }
	[[nodiscard]] Coloru8* GetBuffer()                                const { return this->m_buff.get();                  }
	[[nodiscard]] Coloru8  Sample(const uint16_t x, const uint16_t y) const { return this->m_buff[y * this->m_width + x]; }

	void Set(const uint16_t x, const uint16_t y, const Coloru8& color) { this->m_buff[y * this->m_width + x] = color; }
};