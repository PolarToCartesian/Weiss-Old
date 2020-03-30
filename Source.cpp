#include "Include.h"

int main()
{
	ENABLE_CONSOLE();

	Window* pWindow = Window::Create();
	RenderAPI* renderAPI = RenderAPI::Create(RenderAPIName::DIRECTX11);
	renderAPI->InitRenderAPI(pWindow);

	while (pWindow->IsRunning())
	{
		renderAPI->SwapBuffers();
		pWindow->Update();
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	delete pWindow;
	delete renderAPI;
}