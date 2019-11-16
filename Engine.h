#pragma once

#include "winapi/window/Window.h"

class Engine
{
	public:
		Window* window = nullptr;
		Mouse* mouse;
		Keyboard* keyboard;

		std::unique_ptr<Graphics> graphics;

		Engine(const WindowDescriptor& windowDesc)
		{
			this->window   = WindowManager::createWindow(windowDesc);
			this->graphics = this->window->createGraphics();
			
			this->mouse    = &(this->window->getMouse());
			this->keyboard = &(this->window->getKeyboard());
		}

		void bindCursor() {
			const uint16_t leftX = this->window->getPositionX() + 1;
			const uint16_t topY  = this->window->getPositionY() + 1;

			const uint16_t rightX  = this->window->getPositionX() + this->window->getDimensionW() - 1;
			const uint16_t bottomY = this->window->getPositionY() + this->window->getDimensionH() - 1;

			this->window->getMouse().clip(leftX, rightX, topY, bottomY);
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