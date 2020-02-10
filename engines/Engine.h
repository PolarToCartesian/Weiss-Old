#pragma once

#include "../misc/Colors.h"

#include "../cameras/PerspectiveCamera.h"
#include "../cameras/OrthographicCamera.h"

#include "../renderers/HighLevelRenderer.h"

struct EngineDescriptor
{
	const WindowDescriptor windowDesc;
	const HighLevelRendererDescriptor highLevelRendererDesc;
};

class EngineInitializationException : public std::exception { };

struct DataFromMeshFile
{
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
};

class Engine : public HighLevelRenderer
{
protected:
	size_t windowIndex = 0u;

	Mouse* mouse = nullptr;
	Keyboard* keyboard = nullptr;

public:
	Engine();

	void InitEngine(const EngineDescriptor& desc);

	void CaptureCursor();

	void PlayWavFile(const char* filename);

	[[nodiscard]] DataFromMeshFile LoadDataFromMeshFile(const char* filename);

	[[nodiscard]] Mouse&    GetMouse()    noexcept;
	[[nodiscard]] Window&   GetWindow()   noexcept;
	[[nodiscard]] Engine&   GetEngine()   noexcept;
	[[nodiscard]] Keyboard& GetKeybaord() noexcept;
};