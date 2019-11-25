#pragma once

#include "../misc/Includes.h"

struct VertexShaderDescriptor
{
	const std::vector<std::pair<const char*, DXGI_FORMAT>> inputElementDescriptors;
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