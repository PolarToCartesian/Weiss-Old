#pragma once

#include "Shader.h"
#include "InputLayout.h"

class VertexShaderCreationException : public std::exception { };

class VertexShader : public Shader<ID3D11VertexShader> {
private:
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBlob;

	InputLayout* m_inputLayout;

public:
	VertexShader(const DeviceInfo& deviceInfo, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* sourceFilename);
	~VertexShader();

	virtual void Load() override;

	virtual void Bind() const noexcept override;
};