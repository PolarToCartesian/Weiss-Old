#include "Device.h"

[[nodiscard]] bool VulkanDeviceQueueFamilyIndices::isComplete() const noexcept
{
	return m_graphicsIndex.has_value() && m_presentIndex.has_value();
}

VulkanDevice::VulkanDevice(const VulkanInstance& instance, const VulkanSurface& surface)
	: m_instance(instance), m_surface(surface)
{
	this->FetchRequiredExtensions();
	this->SelectPhysicalDevice();
	this->CreateLogicalDeviceAndQueues();
}

VulkanDevice::~VulkanDevice()
{
	vkDestroyDevice(this->m_device, nullptr);
}

VulkanDevice::operator const VkDevice() const noexcept { return this->m_device; }

[[nodiscard]] VkDevice VulkanDevice::Get() noexcept { return this->m_device; }

[[nodiscard]] VkQueue& VulkanDevice::GetGraphicsQueue() noexcept { return this->m_gfxQueue; }
[[nodiscard]] VkQueue& VulkanDevice::GetPresentQueue()  noexcept { return this->m_presentQueue; }

[[nodiscard]] VkPhysicalDevice& VulkanDevice::GetPhysical() noexcept { return this->m_physicalDevice; }

[[nodiscard]] VulkanDeviceQueueFamilyIndices VulkanDevice::GetQueueFamilyIndices() const noexcept { return this->m_queueFamilyIndices; }

std::vector<VkPhysicalDevice> VulkanDevice::GetPhysicalDevices() noexcept
{
	uint32_t deviceCount = 0u;
	vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, nullptr);

	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(this->m_instance, &deviceCount, physicalDevices.data());

	return physicalDevices;
}

std::vector<VkQueueFamilyProperties> VulkanDevice::GetQueueFamilyProperties(const VkPhysicalDevice& physicalDevice) noexcept
{
	uint32_t familyCount = 0u;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyPropreties(familyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, queueFamilyPropreties.data());

	return queueFamilyPropreties;
}

std::pair<bool, VulkanDeviceQueueFamilyIndices> VulkanDevice::isPhysicalDeviceSuitable(const VkPhysicalDevice& physicalDevice) noexcept
{
	VulkanDeviceQueueFamilyIndices queueFamilyIndices;

	const std::vector<VkQueueFamilyProperties> queueFamilies = GetQueueFamilyProperties(physicalDevice);

	for (uint32_t i = 0u; i < queueFamilies.size(); i++)
	{
		const VkQueueFlags queueFlags = queueFamilies[i].queueFlags;

		if (queueFlags & VK_QUEUE_GRAPHICS_BIT)
			queueFamilyIndices.m_graphicsIndex = i;

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, this->m_surface, &presentSupport);

		if (presentSupport)
			queueFamilyIndices.m_presentIndex = i;
	}

	if (queueFamilyIndices.isComplete() && this->checkExtensionSupport(physicalDevice))
		return { true, queueFamilyIndices };

	return { false, queueFamilyIndices };
}

bool VulkanDevice::checkExtensionSupport(const VkPhysicalDevice& physicalDevice)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	for (const char* extensionName : requiredExtensions)
	{
		bool isAvailable = false;
		for (VkExtensionProperties& availableExtension : availableExtensions)
		{
			if (std::strcmp(availableExtension.extensionName, extensionName) == 0)
			{
				isAvailable = true;
				break;
			}
		}

		if (!isAvailable)
			return false;
	}

	return true;
}

void VulkanDevice::SelectPhysicalDevice()
{
	std::vector<VkPhysicalDevice> physicalDevices = this->GetPhysicalDevices();

	VkPhysicalDevice backupPhysicalDevice = VK_NULL_HANDLE;
	VulkanDeviceQueueFamilyIndices backupQueueFamilyIndices;

	for (const VkPhysicalDevice& physicalDevice : physicalDevices)
	{
		auto [isSupported, queueFamilyIndices] = this->isPhysicalDeviceSuitable(physicalDevice);

		if (isSupported)
		{
			backupPhysicalDevice = physicalDevice;
			backupQueueFamilyIndices = queueFamilyIndices;

			VkPhysicalDeviceProperties physicalDevicePropreties;
			vkGetPhysicalDeviceProperties(physicalDevice, &physicalDevicePropreties);

			if (physicalDevicePropreties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				this->m_physicalDevice = physicalDevice;
				this->m_queueFamilyIndices = queueFamilyIndices;
			}
		}
	}

	if (backupPhysicalDevice == VK_NULL_HANDLE)
		throw std::runtime_error("[VULKAN] No Compatible Physical Device Found");

	this->m_physicalDevice = backupPhysicalDevice;
	this->m_queueFamilyIndices = backupQueueFamilyIndices;
}

void VulkanDevice::CreateLogicalDeviceAndQueues()
{
	const uint32_t nQueues = sizeof(VulkanDeviceQueueFamilyIndices) / sizeof(std::optional<uint32_t>);
	VkDeviceQueueCreateInfo* queueCreateInfos = new VkDeviceQueueCreateInfo[nQueues];

	for (uint16_t i = 0; i < nQueues; i++)
	{
		VkDeviceQueueCreateInfo& qci = queueCreateInfos[i];

		qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		qci.queueFamilyIndex = (reinterpret_cast<std::optional<uint32_t>*>(&m_queueFamilyIndices) + i)->value();
		qci.queueCount = 1u;

		float queuePriority = 1.0f;
		qci.pQueuePriorities = &queuePriority;
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.pQueueCreateInfos = queueCreateInfos;
	createInfo.queueCreateInfoCount = nQueues;
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.enabledLayerCount = 0;

	if (vkCreateDevice(this->m_physicalDevice, &createInfo, nullptr, &this->m_device) != VK_SUCCESS)
		throw std::runtime_error("|VULKAN] Failed To Create Logical Device");

	delete[] queueCreateInfos;

	vkGetDeviceQueue(this->m_device, this->m_queueFamilyIndices.m_graphicsIndex.value(), 0, &this->m_gfxQueue);
	vkGetDeviceQueue(this->m_device, this->m_queueFamilyIndices.m_presentIndex.value(), 0, &this->m_presentQueue);
}

void VulkanDevice::FetchRequiredExtensions() noexcept
{
	requiredExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}