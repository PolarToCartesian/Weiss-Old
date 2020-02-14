#pragma once

#include "../misc/Bindable.h"

template <typename M_T_CREATION_EXCEPTION, typename M_T_MAPPING_EXCEPTION>
class Buffer : public Bindable<ID3D11Buffer> {
protected:
	UINT m_byteWidth;
	UINT m_structuredByteStride;

protected:
	void __Buffer__SetData(const void* buff, const size_t nBytes) const
	{
		D3D11_MAPPED_SUBRESOURCE resource;

		if (this->m_deviceInfo.m_pDeviceContext->Map(this->m_pResource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource) != S_OK)
		{
			throw M_T_MAPPING_EXCEPTION();
		}

		std::memcpy(resource.pData, buff, nBytes);

		this->m_deviceInfo.m_pDeviceContext->Unmap(this->m_pResource.Get(), 0);
	}

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
			throw M_T_CREATION_EXCEPTION();
	}
};