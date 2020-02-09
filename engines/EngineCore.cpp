#include "EngineCore.h"

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

	this->InitializeLowLevelGraphics(Window::m_s_windows[windowIndex]);

	DeviceInfo di = this->GetDeviceInfo();

	this->InitBufferManager(di);
	this->InitShaderManager(di);
	this->InitTextureManager(di);

	this->CreateDefaultConstantBuffers();

	this->GetWindow().OnResize([&](const Vec2u16 dimensions)
	{
		this->InitializeLowLevelGraphics(Window::m_s_windows[windowIndex]);
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

	this->m_vertexBuffers[mesh.vertexBufferIndex].Bind();
	this->m_vertexShaders[mesh.vertexShaderIndex].Bind();
	this->m_pixelShaders[mesh.pixelShaderIndex].Bind();

	for (const size_t textureIndex : mesh.textureIndices)
		this->m_imageTexturePairs[textureIndex].textures[0]->Bind();

	for (const size_t textureSamplerIndex : mesh.textureSamplerIndices)
		this->m_textureSamplers[textureSamplerIndex].Bind();

	for (const size_t cbIndex : mesh.constantBufferIndices)
		this->m_constantBuffers[cbIndex].Bind();

	this->m_pDeviceContext->IASetPrimitiveTopology(mesh.primitiveTopologyType);

	if (mesh.indexBufferIndex.has_value())
	{
		this->m_indexBuffers[mesh.indexBufferIndex.value()].Bind();

		if (count == 0u)
			count = static_cast<UINT>(this->m_indexBuffers[mesh.indexBufferIndex.value()].GetSize());

		this->m_pDeviceContext->DrawIndexed(count, 0u, 0u);
	}
	else
	{
		if (count == 0u)
			count = static_cast<UINT>(this->m_vertexBuffers[mesh.vertexBufferIndex].GetElementCount());

		this->m_pDeviceContext->Draw(count, 0u);
	}
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

Mesh&           EngineCore::GetMesh(const size_t index)           noexcept { return this->meshes[index];             }

// Only Interact With A Window Through Its Index Because The "m_s_windows" Array Changes When New Windows Are Created
Window&     EngineCore::GetWindow()     noexcept { return Window::m_s_windows[this->windowIndex]; }
Mouse&      EngineCore::GetMouse()      noexcept { return *this->mouse;                           }
Keyboard&   EngineCore::GetKeybaord()   noexcept { return *this->keyboard;                        }
EngineCore& EngineCore::GetEngineCore() noexcept { return *this;                                  }