#pragma once

#include "winapi/window/Window.h"

class Engine
{
	public:
		Window* window;
		std::unique_ptr<Graphics> graphics;

		Engine(const WindowDescriptor& windowDesc)
		{
			this->window   = WindowManager::createWindow(windowDesc);
			this->graphics = this->window->createGraphics();
		}

		void update()
		{
			this->window->update();
		}

		void render(const bool useVSync)
		{
			this->graphics->render(useVSync);
		}
};