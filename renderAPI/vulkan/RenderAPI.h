#pragma once

#include "Shaders.h"
#include "internal/Include.h"
#include "../common/Include.h"
#include "../../common/Include.h"

class VulkanRenderAPI : public RenderAPI {
private:
	std::shared_ptr<VulkanDevice> m_pDevice;
	std::shared_ptr<VulkanSurface> m_pSurface;
	std::shared_ptr<VulkanInstance> m_pInstance;
	std::shared_ptr<VulkanSwapChain> m_pSwapChain;

public:
	VulkanRenderAPI();

	virtual void InitRenderAPI(Window* pWindow) override;

	virtual void Draw(const PrimitiveTopology& topology, const size_t nVertices) override;

	virtual void DrawIndexed(const PrimitiveTopology& topology, const size_t nIndices) override;

	virtual void SwapBuffers() override;

	// Defined Later
	virtual VertexShader* CreateVertexShader(const char* sourceFilename, const std::vector<ShaderInputElement>& sies) override;

	virtual PixelShader* CreatePixelShader(const char* sourceFilename) override;
};