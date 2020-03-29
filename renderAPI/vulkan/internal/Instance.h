#pragma once

#include "../../../common/Include.h"

class VulkanInstance {
private:
	VkInstance m_instance = VK_NULL_HANDLE;

public:
	VulkanInstance();

	~VulkanInstance();

	operator const VkInstance() const noexcept;

	VkInstance Get() noexcept;

private:
	static std::vector<VkLayerProperties> GetSupportedLayers();
};