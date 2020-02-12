#pragma once

#include "misc/Colors.h"

#include "cameras/PerspectiveCamera.h"
#include "cameras/OrthographicCamera.h"

#include "renderers/HighLevelRenderer.h"

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

class Weiss : public HighLevelRenderer
{
protected:
	Window* m_window = nullptr;

public:
	Weiss();

	void InitializeWeiss(const EngineDescriptor& desc);

	void CaptureCursor();

	void PlayWavFile(const char* filename);

	[[nodiscard]] DataFromMeshFile LoadDataFromMeshFile(const char* filename);

	[[nodiscard]] Mouse&    GetMouse()    noexcept;
	[[nodiscard]] Window&   GetWindow()   noexcept;
	[[nodiscard]] Weiss&    GetWeiss()    noexcept;
	[[nodiscard]] Keyboard& GetKeybaord() noexcept;
};