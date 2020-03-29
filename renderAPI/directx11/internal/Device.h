#pragma once

#include "../../common/Include.h"
#include "../../../common/Include.h"

#ifdef __WEISS__OS_WINDOWS

class DirectX11Device {
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;

public:
	DirectX11Device()
	{
		if (D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &this->m_pDevice, nullptr, &this->m_pDeviceContext) != S_OK)
			throw std::runtime_error("Could Not Create Device");
	}

	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11Device>& GetDevice() noexcept { return this->m_pDevice; }
	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetDeviceContext() noexcept { return this->m_pDeviceContext; }
};

#endif // __WEISS__OS_WINDOWS