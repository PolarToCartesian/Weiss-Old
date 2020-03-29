#pragma once

#include "Shaders.h"
#include "internal/Include.h"
#include "../common/Include.h"
#include "../../common/Include.h"

#ifdef __WEISS__OS_WINDOWS

class DirectX11RenderAPI : public RenderAPI {
private:
	std::shared_ptr<DirectX11Device> m_pDevice;
	std::shared_ptr<DirectX11SwapChain> m_pSwapChain;
	std::shared_ptr<DirectX11RenderTarget> m_pRenderTarget;

private:
	void SetPrimitiveTopology(const PrimitiveTopology& topology);

public:
	DirectX11RenderAPI();

	virtual void InitRenderAPI(Window* pWindow) override;

	virtual void Draw(const PrimitiveTopology& topology, const size_t nVertices) override;

	virtual void DrawIndexed(const PrimitiveTopology& topology, const size_t nIndices) override;

	virtual void SwapBuffers() override;

	// Defined Later
	virtual VertexShader* CreateVertexShader(const char* sourceFilename, const std::vector<ShaderInputElement>& sies) override;

	virtual PixelShader* CreatePixelShader(const char* sourceFilename) override;

	[[nodiscard]] std::shared_ptr<DirectX11Device> GetDevice() noexcept;
};

#endif // __WEISS__OS_WINDOWS