#pragma once

#include "../misc/Bindable.h"

template <typename M_T_EXCEPTION>
class Buffer : public Bindable<ID3D11Buffer> {
protected:
	UINT m_byteWidth;
	UINT m_structuredByteStride;

public:
	Buffer(const DeviceInfo& deviceInfo, const void* buff, const UINT byteWidth, const UINT structuredByteStride, const D3D11_BIND_FLAG bindFlags, const bool isUpdatable)
		: Bindable(deviceInfo), m_byteWidth(byteWidth), m_structuredByteStride(structuredByteStride)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = bindFlags;
		bd.Usage = (isUpdatable) ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = (isUpdatable) ? D3D11_CPU_ACCESS_WRITE : 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = this->m_byteWidth;
		bd.StructureByteStride = this->m_structuredByteStride;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = buff;

		const D3D11_SUBRESOURCE_DATA* pInitialData = (buff != nullptr) ? &sd : nullptr;
		if (this->m_deviceInfo.m_pDevice->CreateBuffer(&bd, pInitialData, &this->m_pResource) != S_OK)
			throw M_T_EXCEPTION();
	}
};