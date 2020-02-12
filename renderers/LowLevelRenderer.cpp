#include "LowLevelRenderer.h"

void LowLevelRenderer::InitializeLowLevelRenderer(Window* window)
{
	this->m_window = window;

	this->InitializeLowLevelGraphics(this->m_window);

	this->m_deviceInfo = new DeviceInfo(this->GetDeviceInfo());

	this->InitBufferManager(*this->m_deviceInfo);
	this->InitShaderManager(*this->m_deviceInfo);
	this->InitTextureManager(*this->m_deviceInfo);
}

LowLevelRenderer::~LowLevelRenderer()
{
	delete this->m_deviceInfo;
}

void LowLevelRenderer::Run(const bool useVSync, const uint16_t fps)
{
	Timer timer;
	uint32_t frames = 0;

	while (this->m_window->IsRunning())
	{
		const float elapsed = timer.GetElapsedTimeMs();

		if (!useVSync)
		{
			if (elapsed >= 1 / static_cast<float>(fps) * 1000)
			{
				timer = Timer();
			}
			else
			{
				std::this_thread::yield();
				continue;
			}
		}

		this->m_window->Update();

		this->OnRender(elapsed);

		this->PresentFrame(useVSync);
	}
}


void LowLevelRenderer::Draw(const size_t meshIndex, UINT count)
{
	Drawable& drawable = this->drawables[meshIndex];

	if (drawable.vertexBufferIndex == WEISS_NO_RESOURCE_INDEX) return;

	this->m_vertexBuffers[drawable.vertexBufferIndex].Bind();
	this->m_vertexShaders[drawable.vertexShaderIndex].Bind();
	this->m_pixelShaders[drawable.pixelShaderIndex].Bind();

	for (const size_t textureIndex : drawable.textureIndices)
		this->m_imageTexturePairs[textureIndex].textures[0]->Bind();

	for (const size_t textureSamplerIndex : drawable.textureSamplerIndices)
		this->m_textureSamplers[textureSamplerIndex].Bind();

	for (const size_t cbIndex : drawable.constantBufferIndices)
		this->m_constantBuffers[cbIndex].Bind();

	this->m_pDeviceContext->IASetPrimitiveTopology(drawable.primitiveTopologyType);

	if (drawable.indexBufferIndex.has_value())
	{
		this->m_indexBuffers[drawable.indexBufferIndex.value()].Bind();

		if (count == 0u)
			count = static_cast<UINT>(this->m_indexBuffers[drawable.indexBufferIndex.value()].GetSize());

		this->m_pDeviceContext->DrawIndexed(count, 0u, 0u);
	}
	else
	{
		if (count == 0u)
			count = static_cast<UINT>(this->m_vertexBuffers[drawable.vertexBufferIndex].GetElementCount());

		this->m_pDeviceContext->Draw(count, 0u);
	}
}

void LowLevelRenderer::PresentFrame(const bool useVSync)
{
	if (this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Present Frame");
#endif

		throw LowLevelGraphicsInitializerException();
	}

	// Clear Depth Stencil
	this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void LowLevelRenderer::TurnZBufferOn() noexcept
{
	this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOn.Get(), 1u);
}

void LowLevelRenderer::TurnZBufferOff() noexcept
{
	this->m_pDeviceContext->OMSetDepthStencilState(this->m_pDepthStencilStateForZBufferOff.Get(), 1u);
}

[[nodiscard]] size_t LowLevelRenderer::AddDrawable(const Drawable& mesh)
{
	this->drawables.push_back(mesh);

	return this->drawables.size() - 1;
}

[[nodiscard]] Drawable& LowLevelRenderer::GetDrawable(const size_t index) noexcept
{
	return this->drawables[index];
}

[[nodiscard]] LowLevelRenderer& LowLevelRenderer::GetLowLevelRenderer() noexcept
{
	return *this;
}