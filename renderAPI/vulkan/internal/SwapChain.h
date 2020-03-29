#pragma once

#include "Device.h"
#include "Surface.h"
#include "Instance.h"
#include "../../../common/Include.h"

class VulkanSwapChain {
private:
	VkSwapchainKHR m_swapChain = VK_NULL_HANDLE;

	std::vector<VkImage>     m_images;
	std::vector<VkImageView> m_imageViews;

	VkSurfaceFormatKHR m_surfaceFormat;

	VulkanDevice& m_device;
	VulkanSurface& m_surface;

public:
	VulkanSwapChain(VulkanDevice& device, VulkanSurface& surface);

	~VulkanSwapChain();

	operator const VkSwapchainKHR() const noexcept;

	[[nodiscard]] VkSwapchainKHR Get() const noexcept;
	[[nodiscard]] VkSurfaceFormatKHR GetSurfaceFormat() const noexcept;

private:
	void CreateSwapChain();

	void FetchImages();

	void CreateImageViews();

	std::vector<VkPresentModeKHR> GetSupportedPresentingModes() const noexcept;

	VkPresentModeKHR PickPresentingMode() const;

	std::vector<VkSurfaceFormatKHR> GetSupportedSurfaceFormats() const noexcept;

	VkSurfaceFormatKHR PickSurfaceFormat() const;
};