#include "InputLayout.h"

InputLayout::InputLayout(const DeviceInfo& deviceInfo, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const Microsoft::WRL::ComPtr<ID3DBlob>& pBlob)
	: m_deviceInfo(deviceInfo)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptors(ieds.size());
	for (uint32_t i = 0; i < inputElementDescriptors.size(); i++)
	{
		inputElementDescriptors[i] = D3D11_INPUT_ELEMENT_DESC{};
		inputElementDescriptors[i].SemanticName = ieds[i].first;
		inputElementDescriptors[i].Format = ieds[i].second;
		inputElementDescriptors[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElementDescriptors[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	}

	if (this->m_deviceInfo.m_pDevice->CreateInputLayout(inputElementDescriptors.data(), (UINT)inputElementDescriptors.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pInputLayout) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Input Layout");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw InputLayoutCreationException();
	}
}

void InputLayout::Bind() const noexcept
{
	this->m_deviceInfo.m_pDeviceContext->IASetInputLayout(this->m_pInputLayout.Get());
}