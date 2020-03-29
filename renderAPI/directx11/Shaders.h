#pragma once

#include "RenderAPI.h"
#include "../common/Include.h"
#include "../../common/Include.h"

#ifdef __WEISS__OS_WINDOWS

class DirectX11VertexShader : public VertexShader {
private:
	RenderAPI* m_pRenderAPI;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  m_pInputLayout;

public:
	DirectX11VertexShader(RenderAPI* pRenderAPI, const char* sourceFilename, const std::vector<ShaderInputElement>& sies);

	virtual void Bind() override;
};

class DirectX11PixelShader : public PixelShader {
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pShader;

	RenderAPI* m_pRenderAPI;

public:
	DirectX11PixelShader(RenderAPI* pRenderAPI, const char* sourceFilename);

	virtual void Bind() override;
};

#endif // __WEISS__OS_WINDOWS