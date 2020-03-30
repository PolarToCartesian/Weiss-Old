#pragma once

#include "Shaders.h"
#include "internal/Include.h"
#include "../common/Include.h"
#include "../../common/Include.h"

#ifdef __WEISS__OS_WINDOWS

class DirectX11RenderAPI : public RenderAPI {
private:
	std::shared_ptr<DirectX11Device>       m_pDevice;
	std::shared_ptr<DirectX11SwapChain>    m_pSwapChain;
	std::shared_ptr<DirectX11RenderTarget> m_pRenderTarget;
	std::shared_ptr<DirectX11DepthBuffer>  m_pDepthBuffer;

private:
	void SetPrimitiveTopology(const PrimitiveTopology& topology);

public:
	DirectX11RenderAPI();

	virtual void InitRenderAPI(Window* pWindow) override;

	virtual void Draw(const Drawable& drawable, const size_t nVertices) override;

	virtual void SwapBuffers() override;

	virtual size_t CreateShaderPair(const char* vsFilename, const std::vector<ShaderInputElement>& sies, const char* psFilename, const PrimitiveTopology& topology = PrimitiveTopology::TRIANGLES) override;

	[[nodiscard]] std::shared_ptr<DirectX11Device> GetDevice() noexcept;
};

#endif // __WEISS__OS_WINDOWS