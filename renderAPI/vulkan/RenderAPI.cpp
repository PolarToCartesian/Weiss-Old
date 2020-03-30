#include "RenderAPI.h"

VulkanRenderAPI::VulkanRenderAPI() : RenderAPI(RenderAPIName::VULKAN) {  }

void VulkanRenderAPI::InitRenderAPI(Window* pWindow)
{
	this->m_pInstance  = std::make_shared<VulkanInstance>();
	this->m_pSurface   = std::make_shared<VulkanSurface>(*this->m_pInstance, pWindow);
	this->m_pDevice    = std::make_shared<VulkanDevice>(*this->m_pInstance, *this->m_pSurface);
	this->m_pSwapChain = std::make_shared<VulkanSwapChain>(*this->m_pDevice, *this->m_pSurface);
}

void VulkanRenderAPI::Draw(const Drawable& drawable, const size_t nVertices)
{

}

void VulkanRenderAPI::SwapBuffers()
{
	
}

size_t VulkanRenderAPI::CreateShaderPair(const char* vsFilename, const std::vector<ShaderInputElement>& sies, const char* psFilename, const PrimitiveTopology& topology)
{
	this->m_shaderPairs.push_back({
		nullptr,
		nullptr,
		topology
	});

	return this->m_shaderPairs.size() - 1u;
}