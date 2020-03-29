#include "Include.h"

Window* Window::Create(const WindowDescriptor& descriptor)
{
#ifdef __WEISS__OS_WINDOWS
	return new WindowsWindow(descriptor);
#endif // __WEISS__OS_WINDOWS

	throw std::runtime_error("Could Not Create A Window For Your Operating System");
}