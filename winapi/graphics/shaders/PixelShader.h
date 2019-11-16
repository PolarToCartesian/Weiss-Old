#pragma once

#include "../../misc/Error.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")

struct PixelShaderDescriptor
{
	const wchar_t* pixelShaderFilename;
};

class PixelShader {
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, 
					Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
					const PixelShaderDescriptor& descriptor);

		void Bind() const noexcept;
};