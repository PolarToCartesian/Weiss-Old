#include "VertexShader.h"

VertexShader::VertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, 
						   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
						   const VertexShaderDescriptor& descriptor)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	HRESULT_ERROR(D3DReadFileToBlob(descriptor.vertexShaderFilename, &pBlob), "Could Not Read Vertex Shader File");
	HRESULT_ERROR(pDeviceRef->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pVertexShader), "Could Not Create Vertex Shader");

	HRESULT_ERROR(pDeviceRef->CreateInputLayout(
		descriptor.ieds.data(), (UINT)std::size(descriptor.ieds),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&m_pInputLayout
	), "Could Not Create Input Layout");
}

void VertexShader::Bind() const noexcept
{
	this->m_pDeviceContextRef->IASetInputLayout(this->m_pInputLayout.Get());
	this->m_pDeviceContextRef->VSSetShader(this->m_pVertexShader.Get(), nullptr, 0u);
}