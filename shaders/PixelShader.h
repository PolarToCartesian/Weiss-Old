#pragma once

#include "Shader.h"

class PixelShaderCreationException : public std::exception { };

class PixelShader : public Shader<ID3D11PixelShader> {
public:
	PixelShader(const DeviceInfo& deviceInfo, const char* sourceFilename);

	virtual void Load() override;

	virtual void Bind() const noexcept override;
};