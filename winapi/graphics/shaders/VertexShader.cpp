#include "VertexShader.h"

VertexShader::VertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, 
						   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
						   const VertexShaderDescriptor& descriptor)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(descriptor.vertexShaderFilename, &pBlob);
	H_ERROR(pDeviceRef->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pVertexShader));

	H_ERROR(pDeviceRef->CreateInputLayout(
		descriptor.ieds.data(), (UINT)std::size(descriptor.ieds),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&m_pInputLayout
	));
}

void VertexShader::Bind() const noexcept
{
	this->m_pDeviceContextRef->IASetInputLayout(this->m_pInputLayout.Get());
	this->m_pDeviceContextRef->VSSetShader(this->m_pVertexShader.Get(), nullptr, 0u);
}