#pragma once

#include "../../misc/Error.h"
#include "../../../math/Vertex.h"

#include <wrl.h>
#include <vector>
#include <d3d11.h>

class ConstantBuffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

		size_t m_objSize;

	public:
		ConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const size_t objSize);

		void setData(const void* objPtr);

		void Bind() const noexcept;
};