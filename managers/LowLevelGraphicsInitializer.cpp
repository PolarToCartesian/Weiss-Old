#include "LowLevelGraphicsInitializer.h"

void LowLevelGraphicsInitializer::CreateDeviceAndSwapChain(Window* window)
{
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
	scd.OutputWindow = window->GetHandle();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_pSwapChain, &m_pDevice, nullptr, &m_pDeviceContext) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Device And SwapChain");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}
}

void LowLevelGraphicsInitializer::CreateRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	if (this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Get BackBuffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}

	if (this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create RenderTargetView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}
}

void LowLevelGraphicsInitializer::CreateViewport(Window* window)
{
	D3D11_VIEWPORT vp;
	vp.Width  = static_cast<FLOAT>(window->GetClientWidth());
	vp.Height = static_cast<FLOAT>(window->GetClientHeight());
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->m_pDeviceContext->RSSetViewports(1u, &vp);
}

void LowLevelGraphicsInitializer::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOn) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create DepthStencilState");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}

	dsDesc.DepthEnable = FALSE;

	if (this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOff) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create EmptyDepthStencilState");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}

	this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOn.Get(), 1u);
}

void LowLevelGraphicsInitializer::CreateDepthStencil(Window* window)
{
	// Create Depth Texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width  = window->GetClientWidth();
	descDepth.Height = window->GetClientHeight();
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (this->m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Texture2D");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}

	// Create Depth Stencil Texture View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	if (this->m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &this->m_pDepthStencilView) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create DepthStencilView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}

	this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
}


void LowLevelGraphicsInitializer::CreateAndBindBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (this->m_pDevice->CreateBlendState(&blendDesc, &this->m_pBlendState) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could not create Blend State");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw LowLevelGraphicsInitializerException();
	}

	this->m_pDeviceContext->OMSetBlendState(this->m_pBlendState.Get(), nullptr, 0xFFFFFFFFu);
}

void LowLevelGraphicsInitializer::InitializeLowLevelGraphics(Window* window)
{
	this->CreateDeviceAndSwapChain(window);
	this->CreateRenderTarget();
	this->CreateViewport(window);
	this->CreateDepthStencilStates();
	this->CreateDepthStencil(window);
	this->CreateAndBindBlendState();
}

DeviceInfo LowLevelGraphicsInitializer::GetDeviceInfo() noexcept
{
	return DeviceInfo { this->m_pDevice, this->m_pDeviceContext };
}