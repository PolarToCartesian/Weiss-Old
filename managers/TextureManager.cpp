#include "TextureManager.h"

void TextureManager::InitTextureManager(DeviceInfo& deviceInfo)
{
	this->m_deviceInfo = new DeviceInfo(deviceInfo);
}

TextureManager::~TextureManager()
{
	delete this->m_deviceInfo;
}

[[nodiscard]] size_t TextureManager::CreateTextureSampler(const TextureSamplerDescriptor& descriptor)
{
	this->m_textureSamplers.emplace_back(*this->m_deviceInfo, descriptor);

	return this->m_textureSamplers.size() - 1u;
}

[[nodiscard]] Texture2D& TextureManager::GetTexture(const size_t texturePairIndex, const size_t textureIndex) noexcept
{
	return *this->m_imageTexturePairs[texturePairIndex].textures[textureIndex];
}

[[nodiscard]] TextureSampler& TextureManager::GetTextureSampler(const size_t index) noexcept
{
	return this->m_textureSamplers[index];
}