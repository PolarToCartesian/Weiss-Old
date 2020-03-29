#include "RenderAPI.h"

#ifdef __WEISS__OS_WINDOWS

void DirectX11RenderAPI::SetPrimitiveTopology(const PrimitiveTopology& topology)
{
	D3D11_PRIMITIVE_TOPOLOGY d3d11PrimitiveTopology;

	switch (topology)
	{
	case PrimitiveTopology::TRIANGLES:
		d3d11PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	case PrimitiveTopology::TRIANGLE_STRIP:
		d3d11PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		break;
	default:
		throw std::runtime_error("The Primitive Topology Type You Resquested Is Not Supported For DirectX11");
		break;
	}

	this->m_pDevice->GetDeviceContext()->IASetPrimitiveTopology(d3d11PrimitiveTopology);
}

DirectX11RenderAPI::DirectX11RenderAPI() : RenderAPI(RenderAPIName::DIRECTX11) {}

void DirectX11RenderAPI::InitRenderAPI(Window* pWindow)
{
	this->m_pDevice = std::make_shared<DirectX11Device>();
	this->m_pSwapChain = std::make_shared<DirectX11SwapChain>(this->m_pDevice, pWindow);
	this->m_pRenderTarget = std::make_shared<DirectX11RenderTarget>(this->m_pDevice, this->m_pSwapChain);

	this->m_pRenderTarget->SetCurrent();

	// Create Viewport
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(pWindow->GetClientWidth());
	vp.Height = static_cast<FLOAT>(pWindow->GetClientHeight());
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->m_pDevice->GetDeviceContext()->RSSetViewports(1u, &vp);
}

void DirectX11RenderAPI::Draw(const PrimitiveTopology& topology, const size_t nVertices)
{
	this->SetPrimitiveTopology(topology);
	this->m_pDevice->GetDeviceContext()->Draw(nVertices, 0u);
}

void DirectX11RenderAPI::DrawIndexed(const PrimitiveTopology& topology, const size_t nIndices)
{
	this->SetPrimitiveTopology(topology);
	this->m_pDevice->GetDeviceContext()->DrawIndexed(nIndices, 0u, 0u);
}

void DirectX11RenderAPI::SwapBuffers()
{
	this->m_pSwapChain->Present();
}

// Defined Later
VertexShader* DirectX11RenderAPI::CreateVertexShader(const char* sourceFilename, const std::vector<ShaderInputElement>& sies)
{
	return new DirectX11VertexShader(this, sourceFilename, sies);
}

PixelShader* DirectX11RenderAPI::CreatePixelShader(const char* sourceFilename)
{
	return new DirectX11PixelShader(this, sourceFilename);
}

[[nodiscard]] std::shared_ptr<DirectX11Device> DirectX11RenderAPI::GetDevice() noexcept { return this->m_pDevice; }

#endif // __WEISS__OS_WINDOWS