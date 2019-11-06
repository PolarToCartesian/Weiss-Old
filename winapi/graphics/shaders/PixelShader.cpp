#include "PixelShader.h"

PixelShader::PixelShader(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, const LPCWSTR filename)
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	H_ERROR(D3DReadFileToBlob(filename, &pBlob));
	H_ERROR(pDeviceRef->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pPixelShader));
}

void PixelShader::Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef) const noexcept
{
	pDeviceContextRef->PSSetShader(this->m_pPixelShader.Get(), nullptr, 0u);
}