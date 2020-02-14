#pragma once

#include "Pch.h"
#include "DeviceInfo.h"

template <typename T>
class Bindable {
protected:
	const DeviceInfo& m_deviceInfo;

	Microsoft::WRL::ComPtr<T> m_pResource;

public:
	Bindable(const DeviceInfo& deviceInfo)
		: m_deviceInfo(deviceInfo) {  }

	virtual void Bind() const noexcept = 0;
};