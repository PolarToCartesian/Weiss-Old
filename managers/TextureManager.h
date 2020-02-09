#pragma once

#include "../misc/DeviceInfo.h"

#include "../misc/Pch.h"
#include "../media/Image.h"
#include "../media/SpriteSheet2D.h"
#include "../media/TextureSampler.h"

struct ImageTexturePair {
	std::variant<Image, SpriteSheet2D> image;
	Texture2DSettings settings;
	std::vector<Texture2D*> textures;
};

class TextureManager
{
protected:
	std::vector<ImageTexturePair> m_imageTexturePairs;
	std::vector<TextureSampler>   m_textureSamplers;

private:
	DeviceInfo* m_deviceInfo = nullptr;

protected:
	void InitTextureManager(DeviceInfo& deviceInfo);

public:
	~TextureManager();

	template <typename T>
	void GenerateTexture(const size_t pairIndex, const size_t textureIndex = 0u)
	{
		ImageTexturePair& pair = this->m_imageTexturePairs[pairIndex];
		Texture2DDescriptor t2dd{ pair.settings, std::get<T>(pair.image) };

		delete pair.textures[textureIndex];
		pair.textures[textureIndex] = new Texture2D(*this->m_deviceInfo, t2dd);
	}

	template <typename T>
	[[nodiscard]] size_t AddTextureResource(const Texture2DSettings& settings, const T& image)
	{
		Texture2DDescriptor t2dd{ settings, image };
		ImageTexturePair itp = { image, settings };

		itp.textures.push_back(new Texture2D(*this->m_deviceInfo, t2dd));

		this->m_imageTexturePairs.emplace_back(itp);

		return this->m_imageTexturePairs.size() - 1u;
	}
	
	[[nodiscard]] size_t CreateTextureSampler(const TextureSamplerDescriptor& descriptor);

	template <typename T>
	[[nodiscard]] T& GetImage(const size_t texturePairIndex) noexcept
	{
		return std::get<T>(this->m_imageTexturePairs[texturePairIndex].image);
	}

	[[nodiscard]] Texture2D& GetTexture(const size_t texturePairIndex, const size_t textureIndex = 0u) noexcept;

	[[nodiscard]] TextureSampler& GetTextureSampler(const size_t index) noexcept;
};