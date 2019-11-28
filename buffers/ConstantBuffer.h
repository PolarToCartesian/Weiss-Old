#pragma once

#include "../misc/includes.h"

#include "../misc/ShaderBindingType.h"

struct ConstantBufferDescriptor
{
	const ShaderBindingType bindingType;
	const size_t objSize;
	const UINT slotVS;
	const UINT slotPS;
};

class ConstantBuffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

		ConstantBufferDescriptor m_descriptor;

	public:
		ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef,
					   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
					   const ConstantBufferDescriptor& descriptor);

		void setData(const void* objPtr);

		void Bind() const noexcept;
};