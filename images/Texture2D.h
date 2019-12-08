#pragma once

#include "Image.h"
#include "../misc/ShaderBindingType.h"

struct Texture2DDescriptor
{
	const ShaderBindingType bindingType;
	const Image& image;
	const UINT slotVS;
	const UINT slotPS;
	const bool useMipMaps;
};

class Texture2D {
	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;
		
		Texture2DDescriptor m_descriptor;

	public:
		Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const Texture2DDescriptor& descriptor);

		void bind() const noexcept;
};