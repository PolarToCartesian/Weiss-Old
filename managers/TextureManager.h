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
	std::vector<TextureSampler> m_textureSamplers;
	std::unordered_map<std::string, ImageTexturePair> m_imageTexturePairs;

private:
	DeviceInfo* m_deviceInfo = nullptr;

protected:
	void InitTextureManager(DeviceInfo& deviceInfo);

public:
	~TextureManager();

	template <typename T>
	void GenerateTexture(const std::string& textureName, const size_t textureIndex = 0u)
	{
		ImageTexturePair& pair = this->m_imageTexturePairs[textureName];
		Texture2DDescriptor t2dd{ pair.settings, std::get<T>(pair.image) };

		delete pair.textures[textureIndex];
		pair.textures[textureIndex] = new Texture2D(*this->m_deviceInfo, t2dd);
	}

	template <typename T>
	[[nodiscard]] void AddImageResource(const std::string& textureName, const Texture2DSettings& settings, const T& image)
	{
		Texture2DDescriptor t2dd{ settings, image };
		ImageTexturePair itp = { image, settings };

		itp.textures.push_back(new Texture2D(*this->m_deviceInfo, t2dd));

		this->m_imageTexturePairs.insert({ textureName, itp });
	}
	
	[[nodiscard]] size_t CreateTextureSampler(const TextureSamplerDescriptor& descriptor);

	template <typename T>
	[[nodiscard]] T& GetImage(const std::string& textureName) noexcept
	{
		return std::get<T>(this->m_imageTexturePairs.at(textureName).image);
	}

	[[nodiscard]] Texture2D& GetTexture(const std::string& textureName, const size_t textureIndex = 0u) noexcept;

	[[nodiscard]] TextureSampler& GetTextureSampler(const size_t index) noexcept;
};