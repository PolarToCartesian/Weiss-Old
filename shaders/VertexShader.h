#pragma once

#include "../misc/Pch.h"
#include "../misc/ShaderBindingLoading.h"

class VertexShaderCreationException : public std::exception { };

struct VertexShaderDescriptor
{
	std::vector<std::pair<const char*, DXGI_FORMAT>> inputElementDescriptors = {};
	ShaderLoadingMethod loadingMethod  = ShaderLoadingMethod::FROM_BINARY_FILE;
	const wchar_t*            binaryFilename = nullptr; // Ignore if ShaderLoadingMethod is FROM_SOURCE_CODE
	const char*               sourceCode     = nullptr; // Ignore if ShaderLoadingMethod is FROM_BINARY_FILE
};

class VertexShader {
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>   m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>    m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

public:
	VertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
				 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				 const VertexShaderDescriptor& descriptor)
		: m_pDeviceContextRef(pDeviceContextRef)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

		if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_BINARY_FILE)
		{
			if (D3DReadFileToBlob(descriptor.binaryFilename, &pBlob) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Read Vertex Shader File");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw VertexShaderCreationException();
			}
		}
		else if (descriptor.loadingMethod == ShaderLoadingMethod::FROM_SOURCE_CODE)
		{
			if (D3DCompile(descriptor.sourceCode, strlen(descriptor.sourceCode), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &pBlob, NULL) != S_OK)
			{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
				MESSAGE_BOX_ERROR("Could Not Compile Vertex Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

				throw VertexShaderCreationException();
			}
		}
		else
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Your Specified Shader Loading Method Is Not Supported From Vertex Shaders");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexShaderCreationException();
		}

		if (pDeviceRef->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pVertexShader) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Vertex Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexShaderCreationException();
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptors(descriptor.inputElementDescriptors.size());
		for (uint32_t i = 0; i < inputElementDescriptors.size(); i++)
		{
			inputElementDescriptors[i] = D3D11_INPUT_ELEMENT_DESC{};
			inputElementDescriptors[i].SemanticName = descriptor.inputElementDescriptors[i].first;
			inputElementDescriptors[i].Format = descriptor.inputElementDescriptors[i].second;
			inputElementDescriptors[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
			inputElementDescriptors[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		}

		if (pDeviceRef->CreateInputLayout(inputElementDescriptors.data(), (UINT)inputElementDescriptors.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout) != S_OK)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Input Layout");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw VertexShaderCreationException();
		}
	}

	void Bind() const noexcept
	{
		this->m_pDeviceContextRef->IASetInputLayout(this->m_pInputLayout.Get());
		this->m_pDeviceContextRef->VSSetShader(this->m_pVertexShader.Get(), nullptr, 0u);
	}
};