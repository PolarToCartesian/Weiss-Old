#pragma once

#include "../managers/BufferManager.h"
#include "../managers/ShaderManager.h"
#include "../managers/TextureManager.h"
#include "../managers/LowLevelGraphicsManager.h"

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

class EngineCore : public TextureManager, public ShaderManager, public BufferManager, public LowLevelGraphicsManager
{
protected:
	std::vector<Drawable> meshes;

	size_t    windowIndex = 0u;

	Mouse*    mouse    = nullptr;
	Keyboard* keyboard = nullptr;

private:
	void CreateDefaultConstantBuffers();

public:
	EngineCore();

	~EngineCore();

	void InitEngineCore(const WindowDescriptor& windowDesc);

	void Run(const bool useVSync = true, const uint16_t fps = 60);

	virtual void OnRender(const float elapsed) = 0;

	void CaptureCursor();

	void PlayWavFile(const char* filename);

	void DrawMesh(const size_t meshIndex, UINT count = 0u);

	[[nodiscard]] size_t CreateMeshFromVertices(const Drawable& mesh);

	[[nodiscard]] DataFromMeshFile LoadDataFromMeshFile(const char* filename);

	[[nodiscard]] Drawable& GetMesh(const size_t index) noexcept;

	// Only Interact With A Window Through Its Index Because The "m_s_windows" Array Changes When New Windows Are Created
	[[nodiscard]] Window&     GetWindow()     noexcept;
	[[nodiscard]] Mouse&      GetMouse()      noexcept;
	[[nodiscard]] Keyboard&   GetKeybaord()   noexcept;
	[[nodiscard]] EngineCore& GetEngineCore() noexcept;
};