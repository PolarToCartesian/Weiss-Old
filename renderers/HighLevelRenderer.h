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
	Window* m_window;

	OrthographicCamera* m_orthographicCamera = nullptr;
	PerspectiveCamera*  m_perspectiveCamera  = nullptr;

private:
	void InitializeHighLevelRenderer2D(const HighLevelRenderer2DDescriptor& desc);
	void InitializeHighLevelRenderer3D(const HighLevelRenderer3DDescriptor& desc);

public:
	~HighLevelRenderer();

	void InitializeHighLevelRenderer(const HighLevelRendererDescriptor& desc, Window* window);

	void Fill(const Coloru8& color);

	void UpdateCameraConstantBuffer(const Camera* cameraPtr) noexcept;

	[[nodiscard]] ConstantBuffer&     GetCameraConstantBuffer() noexcept;
	[[nodiscard]] HighLevelRenderer&  GetHighLevelRenderer()    noexcept;
	[[nodiscard]] PerspectiveCamera&  GetPerspectiveCamera()    noexcept;
	[[nodiscard]] OrthographicCamera& GetOrthographicCamera()   noexcept;
};