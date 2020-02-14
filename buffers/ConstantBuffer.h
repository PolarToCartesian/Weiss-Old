#pragma once

#include "Buffer.h"
#include "../shaders/ShaderBindingLoading.h"

class ConstantBufferDataSettingException : public std::exception { };
class ConstantBufferCreationException    : public std::exception { };

class ConstantBuffer : public Buffer<ConstantBufferCreationException> {
private:
	ShaderBindingType m_bindingType;
	UINT m_slotVS;
	UINT m_slotPS;
	UINT m_objSize;

public:
	ConstantBuffer(const DeviceInfo& deviceInfo, const ShaderBindingType bindingType, const void* buff, const UINT objSize, const UINT slotVS, const UINT slotPS);

	void SetData(const void* objPtr) const;

	virtual void Bind() const noexcept override;
};