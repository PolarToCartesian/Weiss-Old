#pragma once

#include "Surface.h"
#include "Instance.h"
#include "../../../math/Include.h"
#include "../../../window/Include.h"
#include "../../../common/Include.h"

struct VulkanDeviceQueueFamilyIndices {
	std::optional<uint32_t> m_graphicsIndex;
	std::optional<uint32_t> m_presentIndex;

	[[nodiscard]] bool isComplete() const noexcept;
};

class VulkanDevice {
private:
	VkDevice m_device = VK_NULL_HANDLE;
	VkPhysicalDevice m_physicalDevice;

	const VulkanInstance& m_instance;
	const VulkanSurface& m_surface;

	VkQueue m_gfxQueue;
	VkQueue m_presentQueue;

	VulkanDeviceQueueFamilyIndices m_queueFamilyIndices;

	std::vector<const char*> requiredExtensions;

public:
	VulkanDevice(const VulkanInstance& instance, const VulkanSurface& surface);

	~VulkanDevice();

	operator const VkDevice() const noexcept;

	[[nodiscard]] VkDevice Get() noexcept;

	[[nodiscard]] VkQueue& GetGraphicsQueue() noexcept;
	[[nodiscard]] VkQueue& GetPresentQueue()  noexcept;

	[[nodiscard]] VkPhysicalDevice& GetPhysical() noexcept;

	[[nodiscard]] VulkanDeviceQueueFamilyIndices GetQueueFamilyIndices() const noexcept;

private:
	std::vector<VkPhysicalDevice> GetPhysicalDevices() noexcept;

	std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties(const VkPhysicalDevice& physicalDevice) noexcept;

	std::pair<bool, VulkanDeviceQueueFamilyIndices> isPhysicalDeviceSuitable(const VkPhysicalDevice& physicalDevice) noexcept;

	bool checkExtensionSupport(const VkPhysicalDevice& physicalDevice);

	void SelectPhysicalDevice();

	void CreateLogicalDeviceAndQueues();

	void FetchRequiredExtensions() noexcept;
};