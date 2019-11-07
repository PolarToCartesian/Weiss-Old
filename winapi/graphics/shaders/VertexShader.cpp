#include "VertexShader.h"

VertexShader::VertexShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, 
						   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
						   const std::vector<D3D11_INPUT_ELEMENT_DESC>& ieds,
						   const LPCWSTR filename)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(filename, &pBlob);
	H_ERROR(pDeviceRef->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pVertexShader));

	H_ERROR(pDeviceRef->CreateInputLayout(
		ieds.data(), (UINT)std::size(ieds),
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