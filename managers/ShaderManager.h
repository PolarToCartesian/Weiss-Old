#pragma once

#include "../misc/Pch.h"
#include "../misc/DeviceInfo.h"
#include "../shaders/PixelShader.h"
#include "../shaders/VertexShader.h"

class ShaderManager
{
protected:
	std::vector<PixelShader>  m_pixelShaders;
	std::vector<VertexShader> m_vertexShaders;

private:
	DeviceInfo* m_deviceInfo = nullptr;

protected:
	void InitShaderManager(DeviceInfo& deviceInfo);

public:
	~ShaderManager();

	[[nodiscard]] size_t CreatePixelShader(const PixelShaderDescriptor& descriptor);

	[[nodiscard]] size_t CreateVertexShader(const VertexShaderDescriptor& descriptor);

	[[nodiscard]] PixelShader& GetPixelShader(const size_t index) noexcept;

	[[nodiscard]] VertexShader& GetVertexShader(const size_t index) noexcept;
};