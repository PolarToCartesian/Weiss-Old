#pragma once

#include "Device.h"
#include "../../common/Include.h"
#include "../../../window/Include.h"
#include "../../../common/Include.h"

#ifdef __WEISS__OS_WINDOWS

class DirectX11SwapChain {
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;

	std::shared_ptr<DirectX11Device> m_pDevice;

public:
	DirectX11SwapChain(std::shared_ptr<DirectX11Device>& pDevice, Window* pWindow)
		: m_pDevice(pDevice)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;

		if (this->m_pDevice->GetDevice().As(&dxgiDevice) != S_OK)
			throw std::runtime_error("Could Not Get IDXGIDevice From D3D11Device");

		if (dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()) != S_OK)
			throw std::runtime_error("Could Not Get DXGIAdapter From DXGIDevice");

		if (dxgiAdapter->GetParent(__uuidof(IDXGIFactory), &dxgiFactory) != S_OK)
			throw std::runtime_error("Could Not Get DXGIAdapter's Parent");

		DXGI_SWAP_CHAIN_DESC scd{};
		scd.BufferDesc.Width = 0;
		scd.BufferDesc.Height = 0;
		scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		scd.BufferDesc.RefreshRate.Numerator = 0;
		scd.BufferDesc.RefreshRate.Denominator = 0;
		scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		scd.SampleDesc.Count = 1;
		scd.SampleDesc.Quality = 0;
		scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		scd.BufferCount = 1;
		scd.OutputWindow = reinterpret_cast<WindowsWindow*>(pWindow)->GetHandle();
		scd.Windowed = TRUE;
		scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		scd.Flags = 0;

		dxgiFactory->CreateSwapChain(this->m_pDevice->GetDevice().Get(), &scd, &this->m_pSwapChain);
	}

	void Present()
	{
		if (this->m_pSwapChain->Present(0, 0u) != S_OK)
			throw std::runtime_error("Failed To Swap Buffers");
	}

	[[nodiscard]] Microsoft::WRL::ComPtr<IDXGISwapChain>& Get() noexcept { return this->m_pSwapChain; }
};

#endif // __WEISS__OS_WINDOWS