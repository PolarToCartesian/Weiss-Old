#pragma once

#include "EngineCore.h"

#include "../misc/Colors.h"

#include "../cameras/PerspectiveCamera.h"
#include "../cameras/OrthographicCamera.h"

#include "../batch/ColoredBatch2DRenderer.h"

struct LightingDescriptor
{
	struct {
		Colorf32 ambiantColor; // 4 * 4 = 16 bytes
	} in_vertexShaderConstantBuffer;

	struct {
		char padding[16];
	} in_pixelShaderConstantBuffer;
};

struct Engine2DDescriptor
{
	const OrthographicCameraDescriptor orthographicCameraDesc;
	const LightingDescriptor lightingDesc;
};

struct Engine3DDescriptor
{
	const PerspectiveCameraDescriptor perspectiveCameraDesc;
	const LightingDescriptor lightingDesc;
};

struct EngineDescriptor
{
	const Engine2DDescriptor engine2DDesc;
	const Engine3DDescriptor engine3DDesc;
};

class Engine : public EngineCore
{
public:
	void InitEngine(const EngineDescriptor& desc)
	{
		this->InitEngine2D(desc.engine2DDesc);
		this->InitEngine3D(desc.engine3DDesc);
	}

// RENDERER 2D START
private:
	OrthographicCamera* m_orthographicCamera = nullptr;

	ColoredBatch2DRenderer* m_coloredBatch2DRenderer = nullptr;

	LightingDescriptor m_lighting2DDescriptor;

private:
	void EngineCreateDefaultConstantBuffers()
	{
		const ConstantBufferDescriptor cbd = { ShaderBindingType::VERTEX, sizeof(LightingDescriptor), WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_INDEX, WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_SLOT };

		if (this->CreateConstantBuffer(cbd) != WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_INDEX)
		{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
			MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #1 In Target Position");
#endif // __WEISS_SHOW_DEBUG_ERRORS

			throw EngineInitializationException();
		}
	}

	void InitEngine2DBatchRenderers()
	{
		this->m_coloredBatch2DRenderer = new ColoredBatch2DRenderer(this->GetEngine());
	}

	void InitEngine2D(const Engine2DDescriptor& desc)
	{
		this->EngineCreateDefaultConstantBuffers();
		this->InitEngine2DBatchRenderers();

		this->m_lighting2DDescriptor = desc.lightingDesc;

		this->m_orthographicCamera = new OrthographicCamera(this->GetWindow(), desc.orthographicCameraDesc);
	}

public:
	void SetRenderMode2D()
	{
		// Camera Transform
		ConstantBuffer& cameraTransformConstantBuffer = this->m_constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
		const DirectX::XMMATRIX orthographicCameraTransposedTransform = this->m_orthographicCamera->GetTransposedTransform();
		cameraTransformConstantBuffer.SetData(&orthographicCameraTransposedTransform);
		cameraTransformConstantBuffer.Bind();

		// Lighting (VS)
		ConstantBuffer& lightingConstantBufferInVS = this->m_constantBuffers[WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_INDEX];
		lightingConstantBufferInVS.SetData(&this->m_lighting2DDescriptor.in_vertexShaderConstantBuffer);
		lightingConstantBufferInVS.Bind();

		// Lighting (PS)
		// TODO::

		this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOff.Get(), 1u);
	}

	void Fill(const Coloru8& color)
	{
		float colorf[4] = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f };

		this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&colorf);
	}

	[[nodiscard]] Engine& GetEngine() noexcept { return *this; }

	[[nodiscard]] LightingDescriptor&	  GetLighting2DDescriptor()   noexcept { return this->m_lighting2DDescriptor; }
	[[nodiscard]] OrthographicCamera&	  GetOrthographicCamera()     noexcept { return *this->m_orthographicCamera; }
	[[nodiscard]] ColoredBatch2DRenderer& GetColoredBatchRenderer2D() noexcept { return *this->m_coloredBatch2DRenderer; }
// RENDERER 2D END
// RENDERER 3D START

private:
	PerspectiveCamera* m_perspectiveCamera = nullptr;

private:
	void InitEngine3D(const Engine3DDescriptor& desc)
	{
		this->m_perspectiveCamera = new PerspectiveCamera(this->GetWindow(), desc.perspectiveCameraDesc);
	}

public:
	void SetRenderMode3D()
	{
		ConstantBuffer& cameraTransformConstantBuffer = this->m_constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
		const DirectX::XMMATRIX perspectiveCameraTransposedTransform = this->m_perspectiveCamera->GetTransposedTransform();
		cameraTransformConstantBuffer.SetData(&perspectiveCameraTransposedTransform);
		cameraTransformConstantBuffer.Bind();

		this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOn.Get(), 1u);
	}

	[[nodiscard]] PerspectiveCamera& GetPerspectiveCamera() noexcept { return *this->m_perspectiveCamera; }

// RENDERER 3D END
	~Engine()
	{
		delete this->m_orthographicCamera;
		delete this->m_perspectiveCamera;
	}
};