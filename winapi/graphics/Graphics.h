#pragma once

#include "../../math/Vectors.h"

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")

class Graphics {
	public:
		bool wasDirectX3DInitSuccessful = false;

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;

	private:
		void createDeviceAndSwapChain(HWND windowHandle);

		void createRenderTarget();

    public:
        Graphics(HWND windowHandle);

		void fill(const Color& color);

		void present();
};