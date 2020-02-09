#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
	delete this->m_deviceInfo;
}

void ShaderManager::InitShaderManager(DeviceInfo& deviceInfo)
{
	this->m_deviceInfo = new DeviceInfo(deviceInfo);
}

[[nodiscard]] size_t ShaderManager::CreatePixelShader(const PixelShaderDescriptor& descriptor)
{
	this->m_pixelShaders.emplace_back(*this->m_deviceInfo, descriptor);

	return this->m_pixelShaders.size() - 1u;
}

[[nodiscard]] size_t ShaderManager::CreateVertexShader(const VertexShaderDescriptor& descriptor)
{
	this->m_vertexShaders.emplace_back(*this->m_deviceInfo, descriptor);

	return this->m_vertexShaders.size() - 1u;
}

[[nodiscard]] PixelShader& ShaderManager::GetPixelShader(const size_t index) noexcept
{
	return this->m_pixelShaders[index];
}

[[nodiscard]] VertexShader& ShaderManager::GetVertexShader(const size_t index) noexcept
{
	return this->m_vertexShaders[index];
}