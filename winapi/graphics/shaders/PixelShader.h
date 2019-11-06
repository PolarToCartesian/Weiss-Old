#pragma once

#include "../../misc/Error.h"

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")

class PixelShader {
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;

	public:
		PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, const LPCWSTR filename);

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef) const noexcept;
};