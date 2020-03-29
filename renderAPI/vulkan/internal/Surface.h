#pragma once

#include "Instance.h"
#include "../../../math/Include.h"
#include "../../../window/Include.h"
#include "../../../common/Include.h"

class VulkanSurface {
private:
	VkSurfaceKHR m_surface;
	const VulkanInstance& m_instance;

	Vec2u16 m_dimensions{ 0u, 0u };

public:
	VulkanSurface(const VulkanInstance& instance, const Window* window);

	~VulkanSurface();

	Vec2u16 GetDimensions() const noexcept;

	operator const VkSurfaceKHR() const noexcept;

	VkSurfaceKHR Get() const noexcept;
};