#pragma once

#include "../../math/Vectors.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

class Graphics {
	public:
		bool wasDirectX3DSuccessful = false;

	private:
		ID3D11Device* m_pDevice = nullptr;
		IDXGISwapChain* m_pSwapChain = nullptr;
		ID3D11DeviceContext* m_pDeviceContext = nullptr;
		ID3D11RenderTargetView* m_pRenderTarget = nullptr;

	private:
		void createDeviceAndSwapChain(HWND windowHandle) {
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
			scd.OutputWindow = windowHandle;
			scd.Windowed = TRUE;
			scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			scd.Flags = 0;

			this->wasDirectX3DSuccessful = S_OK == D3D11CreateDeviceAndSwapChain(
				nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,
				0,
				nullptr,
				0,
				D3D11_SDK_VERSION,
				&scd,
				&m_pSwapChain,
				&m_pDevice,
				nullptr,
				&m_pDeviceContext
			);
		}

		void createRenderTarget()
		{
			if (this->wasDirectX3DSuccessful)
			{
				ID3D11Resource* pBackBuffer = nullptr;

				this->wasDirectX3DSuccessful = S_OK == this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));

				if (this->wasDirectX3DSuccessful)
				{
					this->wasDirectX3DSuccessful = S_OK == this->m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &this->m_pRenderTarget);
					pBackBuffer->Release();
				}
			}	
		}

    public:
        Graphics(HWND windowHandle)
        {
			this->createDeviceAndSwapChain(windowHandle);
			this->createRenderTarget();
        }

		void fill(const Color& color)
		{
			this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget, (float*) &color);
		}

		void present()
		{
			if (this->wasDirectX3DSuccessful)
				this->m_pSwapChain->Present(1u, 0u);
		}

        ~Graphics()
        {
			if (this->wasDirectX3DSuccessful)
			{
				this->m_pDevice->Release();
				this->m_pSwapChain->Release();
				this->m_pRenderTarget->Release();
				this->m_pDeviceContext->Release();
			}
        }
};