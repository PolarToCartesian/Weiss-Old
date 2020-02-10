#include "HighLevelRenderer.h"

void HighLevelRenderer::InitializeHighLevelRenderer2D(const HighLevelRenderer2DDescriptor& desc)
{
	this->m_lighting2DDescriptor = desc.lightingDesc;

	this->m_orthographicCamera = new OrthographicCamera(Window::m_s_windows[this->m_windowIndex], desc.orthographicCameraDesc);
}

void HighLevelRenderer::InitializeHighLevelRenderer3D(const HighLevelRenderer3DDescriptor& desc)
{
	this->m_perspectiveCamera = new PerspectiveCamera(Window::m_s_windows[this->m_windowIndex], desc.perspectiveCameraDesc);
}

void HighLevelRenderer::CreateDefaultConstantBuffers()
{
	const ConstantBufferDescriptor cbd = { ShaderBindingType::VERTEX, sizeof(DirectX::XMMATRIX), WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX, WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_SLOT };

	if (this->CreateConstantBuffer(cbd) != WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #0 In Target Position");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw HighLevelRendererException();
	}

	const ConstantBufferDescriptor cbd2 = { ShaderBindingType::VERTEX, sizeof(LightingDescriptor), WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_INDEX, WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_SLOT };

	if (this->CreateConstantBuffer(cbd2) != WEISS_LIGHTING_CONSTANT_BUFFER_IN_VS_INDEX)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #1 In Target Position");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw HighLevelRendererException();
	}
}

HighLevelRenderer::~HighLevelRenderer()
{
	delete this->m_orthographicCamera;
	delete this->m_perspectiveCamera;
}

void HighLevelRenderer::InitializeHighLevelRenderer(const HighLevelRendererDescriptor& desc, const size_t windowIndex)
{
	this->m_windowIndex = windowIndex;

	this->InitializeLowLevelRenderer(this->m_windowIndex);

	this->CreateDefaultConstantBuffers();

	this->InitializeHighLevelRenderer2D(desc.renderer2DDesc);
	this->InitializeHighLevelRenderer3D(desc.renderer3DDesc);
}

void HighLevelRenderer::Fill(const Coloru8& color)
{
	float colorf[4] = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f };

	this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&colorf);
}

void HighLevelRenderer::SetRenderMode2D()
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
	
	this->TurnZBufferOff();
}

void HighLevelRenderer::SetRenderMode3D()
{
	ConstantBuffer& cameraTransformConstantBuffer = this->m_constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
	const DirectX::XMMATRIX perspectiveCameraTransposedTransform = this->m_perspectiveCamera->GetTransposedTransform();
	cameraTransformConstantBuffer.SetData(&perspectiveCameraTransposedTransform);
	cameraTransformConstantBuffer.Bind();

	this->TurnZBufferOn();
}

[[nodiscard]] HighLevelRenderer& HighLevelRenderer::GetHighLevelRenderer() noexcept
{
	return *this;
}

[[nodiscard]] PerspectiveCamera& HighLevelRenderer::GetPerspectiveCamera() noexcept
{
	return *this->m_perspectiveCamera;
}

[[nodiscard]] LightingDescriptor& HighLevelRenderer::GetLighting2DDescriptor() noexcept
{
	return this->m_lighting2DDescriptor;
}

[[nodiscard]] OrthographicCamera& HighLevelRenderer::GetOrthographicCamera() noexcept
{
	return *this->m_orthographicCamera;
}