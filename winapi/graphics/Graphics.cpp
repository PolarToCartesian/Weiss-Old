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

	HRESULT_ERROR(D3D11CreateDeviceAndSwapChain(
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
	), "Could Not Create Device And SwapChain");
}

void Graphics::createRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	HRESULT_ERROR(this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer), "Could Not Get BackBuffer");
	HRESULT_ERROR(this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget), "Could Not Create RenderTargetView");
}

void Graphics::createViewport(HWND windowHandle)
{
	RECT clientRect;
	GetClientRect(windowHandle, &clientRect);

	D3D11_VIEWPORT vp;
	vp.Width  = static_cast<FLOAT>(clientRect.right  - clientRect.left);
	vp.Height = static_cast<FLOAT>(clientRect.bottom - clientRect.top);
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->m_pDeviceContext->RSSetViewports(1u, &vp);
}

void Graphics::createDepthBuffer(HWND windowHandle)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	HRESULT_ERROR(this->m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState), "Could Not Create DepthStencilState");

	// Bind pDSState
	this->m_pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	RECT windowRect;
	GetWindowRect(windowHandle, &windowRect);

	// Create Detph Texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = windowRect.right - windowRect.left;
	descDepth.Height = windowRect.bottom - windowRect.top;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT_ERROR(this->m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil), "Could Not Create Texture2D");
	// Create Depth Stencil Texture View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	HRESULT_ERROR(this->m_pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &this->m_pDepthStencilView
	), "Could Not Create DepthStencilView");

	// Bind Depth Stencil
	this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
}

Graphics::Graphics() {}

Graphics::Graphics(HWND windowHandle)
{
	this->initGraphics(windowHandle);
}

void Graphics::initGraphics(HWND windowHandle)
{
	this->createDeviceAndSwapChain(windowHandle);
	this->createRenderTarget();
	this->createViewport(windowHandle);
	this->createDepthBuffer(windowHandle);
}

Microsoft::WRL::ComPtr<ID3D11Device>&        Graphics::getDevice()        { return this->m_pDevice; }
Microsoft::WRL::ComPtr<ID3D11DeviceContext>& Graphics::getDeviceContext() { return this->m_pDeviceContext; }

void Graphics::fill(const Color& color)
{
	this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&color);
}

void Graphics::render(const bool useVSync)
{
	HRESULT_ERROR(this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u), "Could Not Present Frame");

	// Clear Depth Buffer
	this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}