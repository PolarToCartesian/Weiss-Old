#include "ShaderManager.h"

ShaderManager::~ShaderManager()
{
	delete this->m_deviceInfo;
}

void ShaderManager::InitShaderManager(DeviceInfo& deviceInfo)
{
	this->m_deviceInfo = new DeviceInfo(deviceInfo);
}

[[nodiscard]] size_t ShaderManager::CreatePixelShader(const char* sourceFilename)
{
	this->m_pixelShaders.emplace_back(*this->m_deviceInfo, sourceFilename);

	return this->m_pixelShaders.size() - 1u;
}

[[nodiscard]] size_t ShaderManager::CreateVertexShader(const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* sourceFilename)
{
	this->m_vertexShaders.emplace_back(*this->m_deviceInfo, ieds, sourceFilename);

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