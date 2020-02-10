#pragma once

#include "../renderers/LowLevelRenderer.h"
#include "../window/Window.h"
#include "../misc/Defines.h"

#include "../misc/Timer.h"

#include "../objects/Drawable.h"

class EngineInitializationException : public std::exception { };

struct DataFromMeshFile
{
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
};

class EngineCore : public LowLevelRenderer
{
protected:
	size_t    windowIndex = 0u;

	Mouse*    mouse    = nullptr;
	Keyboard* keyboard = nullptr;

private:
	void CreateDefaultConstantBuffers();

public:
	EngineCore();

	~EngineCore();

	void InitEngineCore(const WindowDescriptor& windowDesc);

	void CaptureCursor();

	void PlayWavFile(const char* filename);

	[[nodiscard]] DataFromMeshFile LoadDataFromMeshFile(const char* filename);

	// Only Interact With A Window Through Its Index Because The "m_s_windows" Array Changes When New Windows Are Created
	[[nodiscard]] Window&     GetWindow()     noexcept;
	[[nodiscard]] Mouse&      GetMouse()      noexcept;
	[[nodiscard]] Keyboard&   GetKeybaord()   noexcept;
	[[nodiscard]] EngineCore& GetEngineCore() noexcept;
};