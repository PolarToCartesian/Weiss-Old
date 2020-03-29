#include "SwapChain.h"

VulkanSwapChain::VulkanSwapChain(VulkanDevice& device, VulkanSurface& surface)
	: m_device(device), m_surface(surface)
{
	this->CreateSwapChain();
	this->FetchImages();
	this->CreateImageViews();
}

VulkanSwapChain::~VulkanSwapChain()
{
	for (VkImageView& imageView : this->m_imageViews)
		vkDestroyImageView(this->m_device, imageView, nullptr);

	vkDestroySwapchainKHR(this->m_device, this->m_swapChain, nullptr);
}

VulkanSwapChain::operator const VkSwapchainKHR() const noexcept { return this->m_swapChain; }

[[nodiscard]] VkSwapchainKHR VulkanSwapChain::Get() const noexcept { return this->m_swapChain; }
[[nodiscard]] VkSurfaceFormatKHR VulkanSwapChain::GetSurfaceFormat() const noexcept { return this->m_surfaceFormat; }

void VulkanSwapChain::CreateSwapChain()
{
	const VkPresentModeKHR presentingMode = this->PickPresentingMode();

	this->m_surfaceFormat = this->PickSurfaceFormat();

	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = this->m_surface;
	createInfo.minImageCount = 2u;
	createInfo.imageFormat = this->m_surfaceFormat.format;
	createInfo.imageColorSpace = this->m_surfaceFormat.colorSpace;
	createInfo.imageExtent.width = this->m_surface.GetDimensions().x;
	createInfo.imageExtent.height = this->m_surface.GetDimensions().y;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (this->m_device.GetGraphicsQueue() == this->m_device.GetPresentQueue())
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0u;
		createInfo.pQueueFamilyIndices = nullptr;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2u;

		const VulkanDeviceQueueFamilyIndices queueFamilyIndices = this->m_device.GetQueueFamilyIndices();

		std::array<uint32_t, 2u> sharingQueueFamilyIndices{
			queueFamilyIndices.m_presentIndex.value(),
			queueFamilyIndices.m_graphicsIndex.value()
		};

		createInfo.pQueueFamilyIndices = sharingQueueFamilyIndices.data();
	}

	createInfo.preTransform = VkSurfaceTransformFlagBitsKHR::VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	createInfo.compositeAlpha = VkCompositeAlphaFlagBitsKHR::VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentingMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(this->m_device, &createInfo, nullptr, &this->m_swapChain) != VK_SUCCESS)
		throw std::runtime_error("[VULKAN] Failed To Create Swap Chain");
}

void VulkanSwapChain::FetchImages()
{
	uint32_t nImages;
	vkGetSwapchainImagesKHR(this->m_device, this->m_swapChain, &nImages, nullptr);

	this->m_images.resize(nImages);
	vkGetSwapchainImagesKHR(this->m_device, this->m_swapChain, &nImages, this->m_images.data());
}

void VulkanSwapChain::CreateImageViews()
{
	this->m_imageViews.resize(this->m_images.size());

	for (size_t i = 0; i < this->m_imageViews.size(); i++)
	{
		VkImageViewCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = this->m_images[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = this->m_surfaceFormat.format;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0u;
		createInfo.subresourceRange.levelCount = 1u;
		createInfo.subresourceRange.baseArrayLayer = 0u;
		createInfo.subresourceRange.layerCount = 1u;

		if (vkCreateImageView(this->m_device, &createInfo, nullptr, &this->m_imageViews[i]) != VK_SUCCESS)
			throw std::runtime_error("[VULKAN] Failed To Create An Image View For The Swap Chain");
	}
}

std::vector<VkPresentModeKHR> VulkanSwapChain::GetSupportedPresentingModes() const noexcept
{
	uint32_t nSupportedPresentingModes;
	vkGetPhysicalDeviceSurfacePresentModesKHR(this->m_device.GetPhysical(), this->m_surface, &nSupportedPresentingModes, nullptr);

	if (nSupportedPresentingModes == 0u) return {};

	std::vector<VkPresentModeKHR> supportedPresentingModes(nSupportedPresentingModes);
	vkGetPhysicalDeviceSurfacePresentModesKHR(this->m_device.GetPhysical(), this->m_surface, &nSupportedPresentingModes, supportedPresentingModes.data());

	return supportedPresentingModes;
}

VkPresentModeKHR VulkanSwapChain::PickPresentingMode() const
{
	const std::vector<VkPresentModeKHR> supportedPresentingModes = this->GetSupportedPresentingModes();

	if (supportedPresentingModes.size() == 0u)
		throw std::runtime_error("[VULKAN] No Compatible Presenting Mode Could Be Found");

	for (const VkPresentModeKHR mode : supportedPresentingModes)
		if (mode == VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR)
			return mode;

	return supportedPresentingModes[0];
}

std::vector<VkSurfaceFormatKHR> VulkanSwapChain::GetSupportedSurfaceFormats() const noexcept
{
	uint32_t nSupportedFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(this->m_device.GetPhysical(), this->m_surface, &nSupportedFormats, nullptr);

	if (nSupportedFormats == 0u) return {};

	std::vector<VkSurfaceFormatKHR> supportedFormats(nSupportedFormats);
	vkGetPhysicalDeviceSurfaceFormatsKHR(this->m_device.GetPhysical(), this->m_surface, &nSupportedFormats, supportedFormats.data());

	return supportedFormats;
}

VkSurfaceFormatKHR VulkanSwapChain::PickSurfaceFormat() const
{
	const std::vector<VkSurfaceFormatKHR> supportedSurfaceFormats = this->GetSupportedSurfaceFormats();

	if (supportedSurfaceFormats.size() == 0u)
		throw std::runtime_error("[VULKAN] No Supported Surface Formats Where Found");

	for (const VkSurfaceFormatKHR surfaceFormat : supportedSurfaceFormats)
		if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			return surfaceFormat;

	return supportedSurfaceFormats[0];
}