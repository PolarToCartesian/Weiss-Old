#include "HighLevelRenderer.h"

void HighLevelRenderer::InitializeHighLevelRenderer2D(const HighLevelRenderer2DDescriptor& desc)
{
	this->m_orthographicCamera = new OrthographicCamera(*this->m_window, desc.orthographicCameraDesc);
}

void HighLevelRenderer::InitializeHighLevelRenderer3D(const HighLevelRenderer3DDescriptor& desc)
{
	this->m_perspectiveCamera = new PerspectiveCamera(*this->m_window, desc.perspectiveCameraDesc);
}

HighLevelRenderer::~HighLevelRenderer()
{
	delete this->m_perspectiveCamera;
	delete this->m_orthographicCamera;
}

void HighLevelRenderer::InitializeHighLevelRenderer(const HighLevelRendererDescriptor& desc, Window* window)
{
	this->m_window = window;

	this->InitializeLowLevelRenderer(this->m_window);

	if (this->CreateConstantBuffer(ShaderBindingType::VERTEX, nullptr, sizeof(DirectX::XMMATRIX), WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_SLOT, 0u) != WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #0 In Target Position");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw HighLevelRendererException();
	}

	this->GetCameraConstantBuffer().Bind();
	this->TurnZBufferOn();

	this->InitializeHighLevelRenderer2D(desc.renderer2DDesc);
	this->InitializeHighLevelRenderer3D(desc.renderer3DDesc);
}

void HighLevelRenderer::Fill(const Coloru8& color)
{
	float colorf[4] = { color.red / 255.f, color.green / 255.f, color.blue / 255.f, color.alpha / 255.f };

	this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&colorf);
}

void HighLevelRenderer::UpdateCameraConstantBuffer(const Camera* cameraPtr) noexcept
{
	this->GetCameraConstantBuffer().SetData(&cameraPtr->GetTransposedTransform());
}

[[nodiscard]] ConstantBuffer& HighLevelRenderer::GetCameraConstantBuffer() noexcept
{
	return this->GetConstantBuffer(WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX);
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