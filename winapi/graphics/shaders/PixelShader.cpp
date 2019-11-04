#include "PixelShader.h"

PixelShader::PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const LPCWSTR filename)
	: m_pDeviceContextRef(pDeviceContextRef)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	H_ERROR(D3DReadFileToBlob(filename, &pBlob));
	H_ERROR(pDeviceRef->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pPixelShader));
}

void PixelShader::Bind() const noexcept
{
	this->m_pDeviceContextRef->PSSetShader(this->m_pPixelShader.Get(), nullptr, 0u);
}