#pragma once

#include "../misc/Timer.h"

#include "../Window/Window.h"

#include "../objects/Drawable.h"

#include "../managers/BufferManager.h"
#include "../managers/ShaderManager.h"
#include "../managers/TextureManager.h"
#include "../managers/LowLevelGraphicsManager.h"

class LowLevelRenderer : public TextureManager, public ShaderManager, public BufferManager, public LowLevelGraphicsManager
{
private:
	size_t m_windowIndex;
	
	DeviceInfo* m_deviceInfo;

	std::vector<Drawable> drawables;

public:
	void InitializeLowLevelRenderer(const size_t windowIndex);

	~LowLevelRenderer();

	void Run(const bool useVSync = true, const uint16_t fps = 60);

	virtual void OnRender(const float elapsed) = 0;

	void Draw(const size_t drawableIndex, UINT count = 0u);

	[[nodiscard]] size_t AddDrawable(const Drawable& drawable);

	[[nodiscard]] Drawable& GetDrawable(const size_t index) noexcept;
};