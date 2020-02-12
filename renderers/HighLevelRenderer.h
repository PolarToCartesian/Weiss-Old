#pragma once

#include "LowLevelRenderer.h"

#include "../cameras/PerspectiveCamera.h"
#include "../cameras/OrthographicCamera.h"

class HighLevelRendererException : std::exception { };

struct HighLevelRenderer2DDescriptor
{
	const OrthographicCameraDescriptor orthographicCameraDesc;
};

struct HighLevelRenderer3DDescriptor
{
	const PerspectiveCameraDescriptor perspectiveCameraDesc;
};

struct HighLevelRendererDescriptor
{
	const HighLevelRenderer2DDescriptor renderer2DDesc;
	const HighLevelRenderer3DDescriptor renderer3DDesc;
};

class HighLevelRenderer : public LowLevelRenderer
{
private:
	size_t m_windowIndex;

	OrthographicCamera* m_orthographicCamera = nullptr;
	PerspectiveCamera*  m_perspectiveCamera  = nullptr;

private:
	void InitializeHighLevelRenderer2D(const HighLevelRenderer2DDescriptor& desc);
	void InitializeHighLevelRenderer3D(const HighLevelRenderer3DDescriptor& desc);

public:
	~HighLevelRenderer();

	void InitializeHighLevelRenderer(const HighLevelRendererDescriptor& desc, const size_t windowIndex);

	void Fill(const Coloru8& color);

	void SetRenderMode2D();
	void SetRenderMode3D();

	void UpdateCameraConstantBuffer() noexcept;

	[[nodiscard]] HighLevelRenderer&      GetHighLevelRenderer()      noexcept;
	[[nodiscard]] PerspectiveCamera&      GetPerspectiveCamera()      noexcept;
	[[nodiscard]] OrthographicCamera&     GetOrthographicCamera()     noexcept;
};