#include "Engine.h"

Engine::Engine()
{
	if (CoInitialize(NULL) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Initialize COM");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}
}

void Engine::InitEngine(const EngineDescriptor& desc)
{
	this->windowIndex = Window::CreateNewWindow(desc.windowDesc);

	this->mouse = &(this->GetWindow().GetMouse());
	this->keyboard = &(this->GetWindow().GetKeyboard());

	this->InitializeHighLevelRenderer(desc.highLevelRendererDesc, this->windowIndex);
}

void Engine::CaptureCursor()
{
	// Clip Cursor
	const RECT windowRect = this->GetWindow().GetWindowRectangle();
	const RECT clientRect = this->GetWindow().GetClientRectangle();

	RECT boundingRect = windowRect;

	boundingRect.top += (windowRect.bottom - windowRect.top) - clientRect.bottom + 10; // padding
	boundingRect.left += 10; // padding
	boundingRect.right -= 10; // padding
	boundingRect.bottom -= 10; // padding

	this->mouse->Clip(boundingRect);

	// Hide Cursor
	this->mouse->Hide();
}

void Engine::PlayWavFile(const char* filename)
{
	if (!PlaySound(TEXT(filename), NULL, SND_ASYNC | SND_FILENAME))
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Play Sound From File");
#endif // __WEISS_SHOW_DEBUG_ERRORS
	}
}

[[nodiscard]] DataFromMeshFile Engine::LoadDataFromMeshFile(const char* filename)
{
	std::ifstream infile(filename);

	std::vector<Vec3f>    vertices;
	std::vector<uint32_t> indices;

	std::string opcode;
	char junkChar;
	float junkFloat;
	std::string line;

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		iss >> opcode;

		if (opcode == "v")
		{
			Vec3f vertex{ 0.f, 0.f, 0.f };

			iss >> vertex.x >> vertex.y >> vertex.z;

			vertices.push_back(vertex);
		}
		else if (opcode == "f")
		{
			const uint16_t spaceCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), ' '));
			const uint16_t slashCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), '/'));

			if (spaceCount == 3)
			{
				uint32_t i1 = 0, i2 = 0, i3 = 0;

				if (slashCount == 0)
					iss >> i1 >> i2 >> i3;
				else if (slashCount == 3)
					iss >> i1 >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> i3;
				else if (slashCount == 6)
					iss >> i1 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i3;

				indices.push_back(i1 - 1);
				indices.push_back(i2 - 1);
				indices.push_back(i3 - 1);
			}
		}
	}

	return { vertices, indices };
}

[[nodiscard]] Mouse& Engine::GetMouse() noexcept
{
	return *this->mouse;
}

[[nodiscard]] Window& Engine::GetWindow() noexcept
{
	return Window::m_s_windows[this->windowIndex];
}

[[nodiscard]] Engine& Engine::GetEngine() noexcept
{
	return *this;
}

[[nodiscard]] Keyboard& Engine::GetKeybaord() noexcept
{
	return *this->keyboard;
}