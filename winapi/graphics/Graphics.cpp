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

	H_ERROR(D3D11CreateDeviceAndSwapChain(
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
	));
}

void Graphics::createRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	H_ERROR(this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	H_ERROR(this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget));
}

void Graphics::createViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = 1920;
	vp.Height = 1080;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->m_pDeviceContext->RSSetViewports(1u, &vp);
}

void Graphics::createDepthBuffer()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	H_ERROR(this->m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState));

	// Bind pDSState
	this->m_pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// Create Detph Texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 1920u;
	descDepth.Height = 1080u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	H_ERROR(this->m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil));

	// Create Depth Stencil Texture View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	H_ERROR(this->m_pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &this->m_pDepthStencilView
	));

	// Bind Depth Stencil
	this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
}

Graphics::Graphics() {}

Graphics::Graphics(HWND windowHandle)
{
	this->createDeviceAndSwapChain(windowHandle);
	this->createRenderTarget();
	this->createViewport();
	this->createDepthBuffer();
}

Microsoft::WRL::ComPtr<ID3D11Device>&        Graphics::getDevice()        { return this->m_pDevice; }
Microsoft::WRL::ComPtr<ID3D11DeviceContext>& Graphics::getDeviceContext() { return this->m_pDeviceContext; }

void Graphics::fill(const Color& color)
{
	this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&color);
}

void Graphics::renderMesh(const Mesh& mesh)
{
	mesh.Bind();

	this->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->m_pDeviceContext->DrawIndexed(mesh.ib.getSize(), 0u, 0u);
}

void Graphics::render(const bool useVSync)
{
	H_ERROR(this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u));

	// Clear Depth Buffer
	this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}