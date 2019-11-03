#include "Graphics.h"

void Graphics::createDeviceAndSwapChain(HWND windowHandle) {
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

	this->wasDirectX3DInitSuccessful = S_OK == D3D11CreateDeviceAndSwapChain(
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

void Graphics::createRenderTarget()
{
	if (this->wasDirectX3DInitSuccessful)
	{
		Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

		this->wasDirectX3DInitSuccessful = S_OK == this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

		if (this->wasDirectX3DInitSuccessful)
			this->wasDirectX3DInitSuccessful = S_OK == this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget);
	}
}

Graphics::Graphics(HWND windowHandle)
{
	this->createDeviceAndSwapChain(windowHandle);
	this->createRenderTarget();
}

void Graphics::fill(const Color& color)
{
	this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&color);
}

void Graphics::present()
{
	if (this->wasDirectX3DInitSuccessful)
		this->m_pSwapChain->Present(1u, 0u);
}