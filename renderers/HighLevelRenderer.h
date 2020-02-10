#pragma once

#include "LowLevelRenderer.h"

#include "../cameras/PerspectiveCamera.h"
#include "../cameras/OrthographicCamera.h"

class HighLevelRendererException : std::exception { };

struct LightingDescriptor
{
	struct {
		Colorf32 ambiantColor; // 4 * 4 = 16 bytes
	} in_vertexShaderConstantBuffer;

	struct {
		char padding[16];
	} in_pixelShaderConstantBuffer;
};

struct HighLevelRenderer2DDescriptor
{
	const OrthographicCameraDescriptor orthographicCameraDesc;
	const LightingDescriptor lightingDesc;
};

struct HighLevelRenderer3DDescriptor
{
	const PerspectiveCameraDescriptor perspectiveCameraDesc;
	const LightingDescriptor lightingDesc;
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
	PerspectiveCamera* m_perspectiveCamera = nullptr;

	LightingDescriptor m_lighting2DDescriptor;

private:
	void InitializeHighLevelRenderer2D(const HighLevelRenderer2DDescriptor& desc);
	void InitializeHighLevelRenderer3D(const HighLevelRenderer3DDescriptor& desc);

	void CreateDefaultConstantBuffers();

public:
	~HighLevelRenderer();

	void InitializeHighLevelRenderer(const HighLevelRendererDescriptor& desc, const size_t windowIndex);

	void Fill(const Coloru8& color);

	void SetRenderMode2D();
	void SetRenderMode3D();

	[[nodiscard]] HighLevelRenderer&      GetHighLevelRenderer()      noexcept;
	[[nodiscard]] PerspectiveCamera&      GetPerspectiveCamera()      noexcept;
	[[nodiscard]] LightingDescriptor&     GetLighting2DDescriptor()   noexcept;
	[[nodiscard]] OrthographicCamera&     GetOrthographicCamera()     noexcept;
};