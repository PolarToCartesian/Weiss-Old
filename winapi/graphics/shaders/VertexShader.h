#pragma once

#include "../../misc/Error.h"

#include <wrl.h>
#include <vector>
#include <d3d11.h>
#include <algorithm>
#include <d3dcompiler.h>

#pragma comment(lib,"D3DCompiler.lib")

struct VertexShaderDescriptor
{
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ieds;
	const wchar_t* vertexShaderFilename;
};

class VertexShader {
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>    m_pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		VertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
					 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
					 const VertexShaderDescriptor& descriptor);

		void Bind() const noexcept;
};