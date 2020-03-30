#include "Include.h"

RenderAPI* RenderAPI::Create(const RenderAPIName& renderAPIName)
{
	switch (renderAPIName)
	{
#ifdef __WEISS__OS_WINDOWS
	case RenderAPIName::DIRECTX11:
		return new DirectX11RenderAPI();
#endif // __WEISS__OS_WINDOWS
	//case RenderAPIName::VULKAN:
	//	return new VulkanRenderAPI();
	default:
		throw std::runtime_error("You Render API Is Not Supported Yet \n");
	}
}