#pragma once

#include "meshes/Mesh.h"
#include "../../math/Vectors.h"

#include <wrl.h>
#include <sstream>
#include <d3d11.h>
#include <fstream>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")

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

		void createViewport();

		void createDepthBuffer();

    public:
		Graphics();
        Graphics(HWND windowHandle);

		Microsoft::WRL::ComPtr<ID3D11Device>& getDevice();
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& getDeviceContext();

		void fill(const Color& color);

		void render(const bool useVSync);
};