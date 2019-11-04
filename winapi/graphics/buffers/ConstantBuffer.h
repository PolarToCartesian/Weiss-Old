#pragma once

#include "../../misc/Error.h"

#include <wrl.h>
#include <d3d11.h>

class ConstantBuffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		template <typename T>
		ConstantBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const T& obj);

		void Bind() const noexcept;
};