#include "Engine.h"

void Engine::createDeviceAndSwapChain() {
	DXGI_SWAP_CHAIN_DESC scd{};
	scd.BufferDesc.Width = 0;
	scd.BufferDesc.Height = 0;
	scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = this->window->getHandle();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	HRESULT_ERROR(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scd,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pDeviceContext
	), "Could Not Create Device And SwapChain");
}

void Engine::createRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	HRESULT_ERROR(this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer), "Could Not Get BackBuffer");
	HRESULT_ERROR(this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget), "Could Not Create RenderTargetView");
}

void Engine::createViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width  = static_cast<FLOAT>(this->window->getClientWidth());
	vp.Height = static_cast<FLOAT>(this->window->getClientHeight());
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->m_pDeviceContext->RSSetViewports(1u, &vp);
}

void Engine::createDepthBuffer()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	HRESULT_ERROR(this->m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState), "Could Not Create DepthStencilState");

	// Bind pDSState
	this->m_pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// Create Detph Texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width  = this->window->getClientWidth();
	descDepth.Height = this->window->getClientHeight();
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	HRESULT_ERROR(this->m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil), "Could Not Create Texture2D");
	// Create Depth Stencil Texture View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	HRESULT_ERROR(this->m_pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &this->m_pDepthStencilView
	), "Could Not Create DepthStencilView");

	// Bind Depth Stencil
	this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
}

void Engine::initGraphics()
{
	this->createDeviceAndSwapChain();
	this->createRenderTarget();
	this->createViewport();
	this->createDepthBuffer();
}

void Engine::drawMesh(const size_t meshIndex)
{
	Mesh& mesh = this->meshes[meshIndex];

	mesh.vb.Bind();
	mesh.ib.Bind();
	this->vertexShaders[mesh.vsIndex].Bind();
	this->pixelShaders[mesh.psIndex].Bind();

	for (size_t textureIndex : mesh.t2dIndices)
		this->textures[textureIndex].bind();

	if (mesh.tsIndex.has_value())
		this->textureSamplers[mesh.tsIndex.value()].bind();

	for (const size_t cbIndex : mesh.cbIndices)
		this->constantBuffers[cbIndex].Bind();

	this->m_pDeviceContext->IASetPrimitiveTopology(mesh.pt);
	this->m_pDeviceContext->DrawIndexed(static_cast<UINT>(mesh.ib.getSize()), 0u, 0u);
}

void Engine::presentFrame(const bool useVSync)
{
	HRESULT_ERROR(this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u), "Could Not Present Frame");

	// Clear Depth Buffer
	this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

Engine::Engine(const WindowDescriptor& windowDesc)
{
	this->window   = WindowManager::createWindow(windowDesc);

	this->mouse    = &(this->window->getMouse());
	this->keyboard = &(this->window->getKeyboard());

	this->initGraphics();

	this->window->onResize([this](const Vec2u16 dimensions) {
		this->initGraphics();
	});
}

void Engine::captureCursor()
{
	// Clip Cursor
	const RECT windowRect = this->window->getWindowRect();
	const RECT clientRect = this->window->getClientRect();

	RECT boundingRect = windowRect;

	boundingRect.top    += (windowRect.bottom - windowRect.top) - clientRect.bottom + 10;
	boundingRect.left   += 10;
	boundingRect.right  -= 10;
	boundingRect.bottom -= 10;

	this->window->getMouse().clip(boundingRect);

	// Hide Cursor

	this->mouse->hide();
}

size_t Engine::loadVertexShaderFromFile(const VertexShaderDescriptor& descriptor)
{
	this->vertexShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->vertexShaders.size() - 1;
}

size_t Engine::loadPixelShaderFromFile(const PixelShaderDescriptor& descriptor)
{
	this->pixelShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->pixelShaders.size() - 1;
}

size_t Engine::loadTextureFromImage(const Texture2DDescriptor& descriptor)
{
	this->textures.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->textures.size() - 1;
}

size_t Engine::loadTextureSampler(const TextureSamplerDescriptor& descriptor)
{
	this->textureSamplers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->textureSamplers.size() - 1;
}

size_t Engine::createConstantBuffer(const ConstantBufferDescriptor& descriptor)
{
	this->constantBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->constantBuffers.size() - 1;
}

size_t Engine::createMeshFromVertices(const MeshDescriptorFromVertices& descriptor)
{
	this->meshes.emplace_back(Mesh{
		VertexBuffer(this->m_pDevice, this->m_pDeviceContext, descriptor.vertexBufferDescriptor),
		IndexBuffer(this->m_pDevice, this->m_pDeviceContext, descriptor.indices),
		descriptor.vertexShaderIndex,
		descriptor.pixelShaderIndex,
		descriptor.t2dIndices,
		descriptor.tsIndex,
		descriptor.constantBufferIndices,
		descriptor.primitiveTopology
	});

	return this->meshes.size() - 1;
}

DataFromMeshFile Engine::loadDataFromMeshFile(const MeshDescriptorFromFile& descriptor)
{
	std::ifstream infile(descriptor.filename);

	std::vector<Vec3f>    vertices;
	std::vector<uint32_t> indices;

	std::string opcode;
	char junkChar;
	float junkFloat;
	std::string line;

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);

		iss >> opcode;

		if (opcode == "v")
		{
			Vec3f vertex{ 0.f, 0.f, 0.f };

			iss >> vertex[0] >> vertex[1] >> vertex[2];

			vertices.push_back(vertex);
		}
		else if (opcode == "f")
		{
			const uint16_t spaceCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), ' '));
			const uint16_t slashCount = static_cast<uint16_t>(std::count(line.begin(), line.end(), '/'));

			if (spaceCount == 3)
			{
				uint32_t i1 = 0, i2 = 0, i3 = 0;

				if (slashCount == 0)
					iss >> i1 >> i2 >> i3;
				else if (slashCount == 3)
					iss >> i1 >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> i3;
				else if (slashCount == 6)
					iss >> i1 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i2 >> junkChar >> junkFloat >> junkChar >> junkFloat >> i3;

				indices.push_back(i1 - 1);
				indices.push_back(i2 - 1);
				indices.push_back(i3 - 1);
			}
		}
	}

	return { vertices, indices };
}

void Engine::fill(const Color& color)
{
	this->m_pDeviceContext->ClearRenderTargetView(this->m_pRenderTarget.Get(), (float*)&color);
}

void Engine::queueMeshForRendering(const size_t mesh)
{
	this->renderPool.push_back(mesh);
}

void Engine::run(const bool useVSync, const uint16_t fps) {
	Timer timer;
	uint32_t frames = 0;

	while (this->window->isRunning())
	{
		const float elapsed = timer.getElapsedTimeMs();

		if (!useVSync)
		{
			if (elapsed >= 1 / static_cast<float>(fps) * 1000)
			{
				timer = Timer();
			} else {
				std::this_thread::yield();
				continue;
			}
		}

		this->window->update();
		
		this->renderPool.clear();
		this->onRender(elapsed);

		for (const size_t mesh : this->renderPool)
			this->drawMesh(mesh);

		this->presentFrame(useVSync);
	}
}