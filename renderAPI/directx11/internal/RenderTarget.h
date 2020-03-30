#pragma once

#include "SwapChain.h"
#include "../../common/Include.h"
#include "../../../common/Include.h"

class DirectX11RenderTarget {
private:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;

	std::shared_ptr<DirectX11Device> m_pDevice;

public:
	DirectX11RenderTarget(std::shared_ptr<DirectX11Device>& pDevice, std::shared_ptr<DirectX11SwapChain>& pSwapChain)
		: m_pDevice(pDevice)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

		if (pSwapChain->Get()->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer) != S_OK)
			throw std::runtime_error("Could Not Get BackBuffer");

		if (this->m_pDevice->GetDevice()->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget) != S_OK)
			throw std::runtime_error("Could Not Create RenderTargetView");
	}

	[[nodiscard]] Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTarget() noexcept { return this->m_pRenderTarget; }

	void SetCurrent() const noexcept
	{
		this->m_pDevice->GetDeviceContext()->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), NULL);
	}
};