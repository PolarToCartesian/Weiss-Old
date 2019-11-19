#pragma once

#include "../../misc/Error.h"
#include "../../misc/includes.h"
#include "../../../math/Vertex.h"

#include <vector>

enum class ConstantBufferShaderBinding {
	VERTEX,
	PIXEL,
	BOTH
};

struct ConstantBufferDescriptor
{
	const ConstantBufferShaderBinding bindingType;
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