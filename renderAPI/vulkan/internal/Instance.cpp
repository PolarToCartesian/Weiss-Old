#include "Instance.h"

VulkanInstance::VulkanInstance()
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Weiss";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Weiss Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions;

#ifdef _DEBUG

	extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

#endif // _DEBUG

#ifdef __WEISS__OS_WINDOWS

	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);

#endif // __WEISS__OS_WINDOWS

	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	createInfo.enabledLayerCount = 0u;

	if (vkCreateInstance(&createInfo, nullptr, &this->m_instance) != VK_SUCCESS)
		throw std::runtime_error("[VULKAN] Failed To Create Instance");
}

VulkanInstance::~VulkanInstance()
{
	if (this->m_instance != VK_NULL_HANDLE)
		vkDestroyInstance(this->m_instance, nullptr);
}

VulkanInstance::operator const VkInstance () const noexcept { return this->m_instance; }

VkInstance VulkanInstance::Get() noexcept { return this->m_instance; }

std::vector<VkLayerProperties> VulkanInstance::GetSupportedLayers()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	return availableLayers;
}