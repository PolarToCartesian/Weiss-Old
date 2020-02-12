#include "HighLevelRenderer.h"

void HighLevelRenderer::InitializeHighLevelRenderer2D(const HighLevelRenderer2DDescriptor& desc)
{
	this->m_orthographicCamera = new OrthographicCamera(Window::m_s_windows[this->m_windowIndex], desc.orthographicCameraDesc);
}

void HighLevelRenderer::InitializeHighLevelRenderer3D(const HighLevelRenderer3DDescriptor& desc)
{
	this->m_perspectiveCamera = new PerspectiveCamera(Window::m_s_windows[this->m_windowIndex], desc.perspectiveCameraDesc);
}

HighLevelRenderer::~HighLevelRenderer()
{
	delete this->m_perspectiveCamera;
	delete this->m_orthographicCamera;
}

void HighLevelRenderer::InitializeHighLevelRenderer(const HighLevelRendererDescriptor& desc, const size_t windowIndex)
{
	this->m_windowIndex = windowIndex;

	this->InitializeLowLevelRenderer(this->m_windowIndex);

	const ConstantBufferDescriptor cbd = { ShaderBindingType::VERTEX, sizeof(DirectX::XMMATRIX), WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX, WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_SLOT };

	if (this->CreateConstantBuffer(cbd) != WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #0 In Target Position");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw HighLevelRendererException();
	}

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

	this->TurnZBufferOff();
}

void HighLevelRenderer::SetRenderMode3D()
{
	ConstantBuffer& cameraTransformConstantBuffer = this->m_constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
	cameraTransformConstantBuffer.Bind();

	this->TurnZBufferOn();
}

void HighLevelRenderer::UpdateCameraConstantBuffer() noexcept
{
	ConstantBuffer& cameraTransformConstantBuffer = this->m_constantBuffers[WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX];
	const DirectX::XMMATRIX perspectiveCameraTransposedTransform = this->m_perspectiveCamera->GetTransposedTransform();
	cameraTransformConstantBuffer.SetData(&perspectiveCameraTransposedTransform);
}

[[nodiscard]] HighLevelRenderer& HighLevelRenderer::GetHighLevelRenderer() noexcept
{
	return *this;
}

[[nodiscard]] PerspectiveCamera& HighLevelRenderer::GetPerspectiveCamera() noexcept
{
	return *this->m_perspectiveCamera;
}

[[nodiscard]] OrthographicCamera& HighLevelRenderer::GetOrthographicCamera() noexcept
{
	return *this->m_orthographicCamera;
}