#include "EngineCore.h"

void EngineCore::CreateDeviceAndSwapChain()
{
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
	scd.OutputWindow = this->GetWindow().GetHandle();
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	if (D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &scd, &m_pSwapChain, &m_pDevice, nullptr, &m_pDeviceContext) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Device And SwapChain");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}
}

void EngineCore::CreateRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	if (this->m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Get BackBuffer");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}

	if (this->m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &this->m_pRenderTarget) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create RenderTargetView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}
}

void EngineCore::CreateViewport()
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(this->GetWindow().GetClientWidth());
	vp.Height = static_cast<FLOAT>(this->GetWindow().GetClientHeight());
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	this->m_pDeviceContext->RSSetViewports(1u, &vp);
}

void EngineCore::CreateDepthStencilStates()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if (this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOn) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create DepthStencilState");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}

	dsDesc.DepthEnable = FALSE;

	if (this->m_pDevice->CreateDepthStencilState(&dsDesc, &this->m_pDepthStencilStateForZBufferOff) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create EmptyDepthStencilState");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}
}

void EngineCore::CreateDepthStencil()
{
	// Create Depth Texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = this->GetWindow().GetClientWidth();
	descDepth.Height = this->GetWindow().GetClientHeight();
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (this->m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Texture2D");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}

	// Create Depth Stencil Texture View
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	if (this->m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &this->m_pDepthStencilView) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create DepthStencilView");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}
}

void EngineCore::BindDepthStencil()
{
	this->m_pDeviceContext->OMSetRenderTargets(1u, this->m_pRenderTarget.GetAddressOf(), this->m_pDepthStencilView.Get());
}

void EngineCore::CreateAndBindBlendState()
{
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	if (this->m_pDevice->CreateBlendState(&blendDesc, &this->m_pBlendState) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could not create Blend State");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}

	this->m_pDeviceContext->OMSetBlendState(this->m_pBlendState.Get(), nullptr, 0xFFFFFFFFu);
}

/*
 * This function initializes DirectX and creates key components
*/
void EngineCore::InitGraphics()
{
	this->CreateDeviceAndSwapChain();
	this->CreateRenderTarget();
	this->CreateViewport();
	this->CreateDepthStencilStates();
	this->CreateDepthStencil();
	this->BindDepthStencil();
	this->CreateAndBindBlendState();
}

/*
 * This functions swaps the back buffer and the front buffer to show the frame to the user
 * It also clears the depthStencilView
 */
void EngineCore::PresentFrame(const bool useVSync)
{
	if (this->m_pSwapChain->Present(useVSync ? 1u : 0u, 0u) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Present Frame");
#endif

		throw EngineInitializationException();
	}

	// Clear Depth Stencil
	this->m_pDeviceContext->ClearDepthStencilView(this->m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void EngineCore::CreateDefaultConstantBuffers()
{
	const ConstantBufferDescriptor cbd = { ShaderBindingType::VERTEX, sizeof(DirectX::XMMATRIX), WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX, WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_SLOT };

	if (this->CreateConstantBuffer(cbd) != WEISS_CAMERA_TRANSFORM_CONSTANT_BUFFER_INDEX)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Default Constant Buffer #0 In Target Position");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}
}

EngineCore::EngineCore()
{
	if (CoInitialize(NULL) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Initialize COM");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw EngineInitializationException();
	}
}

EngineCore::~EngineCore()
{
	// No Need To Destroy Window Because It Destroys Itself
}

void EngineCore::InitEngineCore(const WindowDescriptor& windowDesc)
{
	this->windowIndex = Window::CreateNewWindow(windowDesc);

	this->mouse = &(this->GetWindow().GetMouse());
	this->keyboard = &(this->GetWindow().GetKeyboard());

	this->InitGraphics();
	this->CreateDefaultConstantBuffers();

	this->GetWindow().OnResize([this](const Vec2u16 dimensions)
		{
			this->InitGraphics();
		});
}

void EngineCore::Run(const bool useVSync, const uint16_t fps)
{
	Timer timer;
	uint32_t frames = 0;

	while (this->GetWindow().IsRunning())
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

		this->GetWindow().Update();

		this->OnRender(elapsed);

		this->PresentFrame(useVSync);
	}
}

void EngineCore::CaptureCursor()
{
	// Clip Cursor
	const RECT windowRect = this->GetWindow().GetWindowRectangle();
	const RECT clientRect = this->GetWindow().GetClientRectangle();

	RECT boundingRect = windowRect;

	boundingRect.top += (windowRect.bottom - windowRect.top) - clientRect.bottom + 10; // padding
	boundingRect.left += 10; // padding
	boundingRect.right -= 10; // padding
	boundingRect.bottom -= 10; // padding

	this->mouse->Clip(boundingRect);

	// Hide Cursor
	this->mouse->Hide();
}

void EngineCore::PlayWavFile(const char* filename)
{
	if (!PlaySound(TEXT(filename), NULL, SND_ASYNC | SND_FILENAME))
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Play Sound From File");
#endif // __WEISS_SHOW_DEBUG_ERRORS
	}
}

void EngineCore::DrawMesh(const size_t meshIndex, UINT count)
{
	Mesh& mesh = this->meshes[meshIndex];

	if (mesh.vertexBufferIndex == WEISS_NO_RESOURCE_INDEX) return;

	this->vertexBuffers[mesh.vertexBufferIndex].Bind();
	this->vertexShaders[mesh.vertexShaderIndex].Bind();
	this->pixelShaders[mesh.pixelShaderIndex].Bind();

	for (const size_t textureIndex : mesh.textureIndices)
		this->textures[textureIndex].Bind();

	for (const size_t textureSamplerIndex : mesh.textureSamplerIndices)
		this->textureSamplers[textureSamplerIndex].Bind();

	for (const size_t cbIndex : mesh.constantBufferIndices)
		this->constantBuffers[cbIndex].Bind();

	this->m_pDeviceContext->IASetPrimitiveTopology(mesh.primitiveTopologyType);

	if (mesh.indexBufferIndex.has_value())
	{
		this->indexBuffers[mesh.indexBufferIndex.value()].Bind();

		if (count == 0u)
			count = static_cast<UINT>(this->indexBuffers[mesh.indexBufferIndex.value()].GetSize());

		this->m_pDeviceContext->DrawIndexed(count, 0u, 0u);
	}
	else
	{
		if (count == 0u)
			count = static_cast<UINT>(this->vertexBuffers[mesh.vertexBufferIndex].GetElementCount());

		this->m_pDeviceContext->Draw(count, 0u);
	}
}

size_t EngineCore::CreateVertexShader(const VertexShaderDescriptor& descriptor)
{
	this->vertexShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->vertexShaders.size() - 1;
}

size_t EngineCore::CreatePixelShader(const PixelShaderDescriptor& descriptor)
{
	this->pixelShaders.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->pixelShaders.size() - 1;
}

size_t EngineCore::CreateTextureFromImage(const Texture2DDescriptor& descriptor)
{
	this->textures.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->textures.size() - 1;
}

size_t EngineCore::CreateTextureSampler(const TextureSamplerDescriptor& descriptor)
{
	this->textureSamplers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->textureSamplers.size() - 1;
}

size_t EngineCore::CreateConstantBuffer(const ConstantBufferDescriptor& descriptor)
{
	this->constantBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->constantBuffers.size() - 1;
}

size_t EngineCore::CreateVertexBuffer(const VertexBufferDescriptor& descriptor)
{
	this->vertexBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->vertexBuffers.size() - 1;
}

size_t EngineCore::CreateIndexBuffer(const IndexBufferDescriptor& descriptor)
{
	this->indexBuffers.emplace_back(this->m_pDevice, this->m_pDeviceContext, descriptor);

	return this->indexBuffers.size() - 1;
}

size_t EngineCore::CreateMeshFromVertices(const Mesh& mesh)
{
	this->meshes.push_back(mesh);

	return this->meshes.size() - 1;
}

DataFromMeshFile EngineCore::LoadDataFromMeshFile(const char* filename)
{
	std::ifstream infile(filename);

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

			iss >> vertex.x >> vertex.y >> vertex.z;

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

Mesh&           EngineCore::GetMesh(const size_t index)           noexcept { return this->meshes[index];          }
Texture2D&      EngineCore::GetTexture(const size_t index)        noexcept { return this->textures[index];        }
VertexShader&   EngineCore::GetVertexShader(const size_t index)   noexcept { return this->vertexShaders[index];   }
PixelShader&    EngineCore::GetPixelShader(const size_t index)    noexcept { return this->pixelShaders[index];    }
ConstantBuffer& EngineCore::GetConstantBuffer(const size_t index) noexcept { return this->constantBuffers[index]; }
VertexBuffer&   EngineCore::GetVertexBuffer(const size_t index)   noexcept { return this->vertexBuffers[index];   }
IndexBuffer&    EngineCore::GetIndexBuffer(const size_t index)    noexcept { return this->indexBuffers[index];    }
TextureSampler& EngineCore::GetTextureSampler(const size_t index) noexcept { return this->textureSamplers[index]; }

// Only Interact With A Window Through Its Index Because The "m_s_windows" Array Changes When New Windows Are Created
Window&     EngineCore::GetWindow()     noexcept { return Window::m_s_windows[this->windowIndex]; }
Mouse&      EngineCore::GetMouse()      noexcept { return *this->mouse;                           }
Keyboard&   EngineCore::GetKeybaord()   noexcept { return *this->keyboard;                        }
EngineCore& EngineCore::GetEngineCore() noexcept { return *this;                                  }