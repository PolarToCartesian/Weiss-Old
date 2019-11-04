#pragma once

#include "Buffer.h"

class ConstantBuffer : Buffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		template <typename T>
		ConstantBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const T& obj);

		void Bind() override;
};