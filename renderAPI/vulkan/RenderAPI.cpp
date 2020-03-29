#include "RenderAPI.h"

VulkanRenderAPI::VulkanRenderAPI() : RenderAPI(RenderAPIName::VULKAN) {  }

void VulkanRenderAPI::InitRenderAPI(Window* pWindow)
{
	this->m_pInstance  = std::make_shared<VulkanInstance>();
	this->m_pSurface   = std::make_shared<VulkanSurface>(*this->m_pInstance, pWindow);
	this->m_pDevice    = std::make_shared<VulkanDevice>(*this->m_pInstance, *this->m_pSurface);
	this->m_pSwapChain = std::make_shared<VulkanSwapChain>(*this->m_pDevice, *this->m_pSurface);
}

void VulkanRenderAPI::Draw(const PrimitiveTopology& topology, const size_t nVertices)
{

}

void VulkanRenderAPI::DrawIndexed(const PrimitiveTopology& topology, const size_t nIndices)
{

}

void VulkanRenderAPI::SwapBuffers()
{

}

// Defined Later
VertexShader* VulkanRenderAPI::CreateVertexShader(const char* sourceFilename, const std::vector<ShaderInputElement>& sies)
{
	return nullptr;
}

PixelShader* VulkanRenderAPI::CreatePixelShader(const char* sourceFilename)
{
	return nullptr;
}