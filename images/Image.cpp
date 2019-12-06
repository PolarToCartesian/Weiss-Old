#include "Image.h"

Image::Image(const wchar_t* filename)
{
	Microsoft::WRL::ComPtr<IWICBitmapSource>      decodedConvertedFrame;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder>     bitmapDecoder;
	Microsoft::WRL::ComPtr<IWICImagingFactory>    factory;
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frameDecoder;

	HRESULT_ERROR(CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&factory)
	), "Could Not Create IWICImagingFactory");

	HRESULT_ERROR(factory->CreateDecoderFromFilename(
		filename,
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&bitmapDecoder
	), "Could Not Read/Open Image");

	HRESULT_ERROR(bitmapDecoder->GetFrame(0, &frameDecoder));

	HRESULT_ERROR(frameDecoder->GetSize(
		(UINT*)&this->m_width,
		(UINT*)&this->m_height
	), "Could Not Get Image Width/Height");

	this->m_nPixels = this->m_width * this->m_height;

	HRESULT_ERROR(WICConvertBitmapSource(
		GUID_WICPixelFormat32bppRGBA,
		frameDecoder.Get(), 
		&decodedConvertedFrame
	), "Could Not Create Bitmap Converter");

	this->m_buff = std::make_unique<Coloru8[]>(this->m_nPixels * sizeof(Coloru8));

	const WICRect sampleRect { 0, 0, this->m_width, this->m_height };

	HRESULT_ERROR(decodedConvertedFrame->CopyPixels(
		&sampleRect,
		this->m_width   * sizeof(Coloru8), 
		this->m_nPixels * sizeof(Coloru8), 
		(BYTE*)this->m_buff.get()
	), "Could Not Copy Pixels From Bitmap");
}

uint16_t Image::getWidth()   const { return this->m_width;   }
uint16_t Image::getHeight()  const { return this->m_height;  }
uint32_t Image::getNPixels() const { return this->m_nPixels; }

Coloru8 Image::sample(const uint16_t x, const uint16_t y) const                 { return this->m_buff[y * this->m_width + x]; }
void    Image::set   (const uint16_t x, const uint16_t y, const Coloru8& color) { this->m_buff[y * this->m_width + x] = color; }

Coloru8* Image::getBuffer() const { return this->m_buff.get(); }