#include "Surface.h"

VulkanSurface::VulkanSurface(const VulkanInstance& instance, const Window* window)
	: m_instance(instance)
{
#ifdef __WEISS__OS_WINDOWS

	VkWin32SurfaceCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.hwnd = reinterpret_cast<const WindowsWindow*>(window)->GetHandle();
	createInfo.hinstance = GetModuleHandle(nullptr);

	this->m_dimensions.x = window->GetClientWidth();
	this->m_dimensions.y = window->GetClientWidth();

	if (vkCreateWin32SurfaceKHR(this->m_instance, &createInfo, nullptr, &this->m_surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface!");

#endif // __WEISS__OS_WINDOWS
}

VulkanSurface::~VulkanSurface()
{
	vkDestroySurfaceKHR(this->m_instance, this->m_surface, nullptr);
}

Vec2u16 VulkanSurface::GetDimensions() const noexcept { return this->m_dimensions; }

VulkanSurface::operator const VkSurfaceKHR() const noexcept { return this->m_surface; }

VkSurfaceKHR VulkanSurface::Get() const noexcept { return this->m_surface; }