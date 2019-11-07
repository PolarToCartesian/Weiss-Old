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

Graphics::Graphics(HWND windowHandle)
{
	this->createDeviceAndSwapChain(windowHandle);
	this->createRenderTarget();
	this->createViewport();
}

Mesh Graphics::createMesh(const std::vector<Vertex>& vertices,
						  const std::vector<unsigned short>& indices,
						  const std::vector<D3D11_INPUT_ELEMENT_DESC> ieds,
						  const wchar_t* vertexShaderFilename,
						  const wchar_t* pixelShaderFilename,
						  const std::optional<UINT>& objSize)
{
	if (objSize.has_value())
	{
		return Mesh{
			VertexBuffer  (this->m_pDevice, this->m_pDeviceContext, vertices),
			IndexBuffer   (this->m_pDevice, this->m_pDeviceContext, indices),
			VertexShader  (this->m_pDevice, this->m_pDeviceContext, ieds, vertexShaderFilename),
			PixelShader   (this->m_pDevice, this->m_pDeviceContext, pixelShaderFilename),
			ConstantBuffer(this->m_pDevice, this->m_pDeviceContext, objSize.value())
		};
	} else {
		return Mesh{
			VertexBuffer(this->m_pDevice, this->m_pDeviceContext, vertices),
			IndexBuffer (this->m_pDevice, this->m_pDeviceContext, indices),
			VertexShader(this->m_pDevice, this->m_pDeviceContext, ieds, vertexShaderFilename),
			PixelShader (this->m_pDevice, this->m_pDeviceContext, pixelShaderFilename),
		};
	}
}

void Graphics::fill(const Color& color)
{
	this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&color);
}

void Graphics::renderMesh(const Mesh& mesh)
{
	mesh.Bind();

	this->m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), nullptr);

	this->m_pDeviceContext->DrawIndexed(mesh.ib.getSize(), 0u, 0u);
}

void Graphics::present()
{
	H_ERROR(this->m_pSwapChain->Present(1u, 0u));
}