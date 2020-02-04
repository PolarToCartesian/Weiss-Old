#pragma once

#include "../misc/Pch.h"
#include "../misc/ShaderBindingLoading.h"

class ConstantBufferCreationException : public std::exception { };

struct ConstantBufferDescriptor
{
	const ShaderBindingType bindingType = ShaderBindingType::VERTEX;
	const size_t            objSize     = 0u;
	const UINT              slotVS      = 0u; // Ignored if ShaderBindingType is PIXEL
	const UINT              slotPS      = 0u; // Ignored if ShaderBindingType is VERTEX
};

class ConstantBuffer {
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	ConstantBufferDescriptor m_descriptor;

public:
	ConstantBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>&		  pDeviceRef,
						 Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
				   const ConstantBufferDescriptor&					  descriptor);

	void SetData(const void* objPtr);

	void Bind() const noexcept;
};