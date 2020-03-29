#pragma once

#include "Shaders.h"
#include "../../common/Include.h"
#include "../../window/Include.h"

class RenderAPI {
private:
	RenderAPIName m_apiName;

public:
	RenderAPI(const RenderAPIName& apiName) : m_apiName(apiName) {  }

	[[nodiscard]] RenderAPIName GetRenderAPIName() const noexcept { return this->m_apiName; }

	// ----- Virtual Functions ----- //

    virtual void InitRenderAPI(Window* pWindow) = 0;

    virtual void Draw(const PrimitiveTopology& topology, const size_t nVertices) = 0;
    virtual void DrawIndexed(const PrimitiveTopology& topology, const size_t nIndices) = 0;

    virtual void SwapBuffers() = 0;

	virtual VertexShader* CreateVertexShader(const char* sourceFilename, const std::vector<ShaderInputElement>& sies) = 0;
	virtual PixelShader*  CreatePixelShader(const char* sourceFilename) = 0;

public:
    // Defined Per Render API
    static RenderAPI* Create(const RenderAPIName& renderAPIName);
};