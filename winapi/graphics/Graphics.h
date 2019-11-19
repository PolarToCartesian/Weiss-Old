#pragma once

#include "meshes/Mesh.h"
#include "../misc/includes.h"
#include "../../math/Vectors.h"

#include <sstream>
#include <fstream>

class Graphics {
	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	private:
		void createDeviceAndSwapChain(HWND windowHandle);

		void createRenderTarget();

		void createViewport(HWND windowHandle);

		void createDepthBuffer(HWND windowHandle);

    public:
		Graphics();
        Graphics(HWND windowHandle);

		void initGraphics(HWND windowHandle);

		Microsoft::WRL::ComPtr<ID3D11Device>& getDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& getDeviceContext();

		void fill(const Color& color);

		void render(const bool useVSync);
};