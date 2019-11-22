#include "Engine.h"

Engine::Engine(const WindowDescriptor& windowDesc)
{
	this->window = WindowManager::createWindow(windowDesc);
	this->graphics = this->window->createGraphics();

	this->mouse = &(this->window->getMouse());
	this->keyboard = &(this->window->getKeyboard());

	this->window->onResize([this](const Vec2u dimensions) {
		this->graphics->initGraphics(this->window->getHandle());
	});
}

void Engine::captureCursor()
{
	// Clip Cursor
	const RECT windowRect = this->window->getWindowRect();
	const RECT clientRect = this->window->getClientRect();

	RECT boundingRect = windowRect;

	boundingRect.top += (windowRect.bottom - windowRect.top) - clientRect.bottom;

	this->window->getMouse().clip(boundingRect);

	// Hide Cursor

	this->mouse->hide();
}

size_t Engine::loadVertexShaderFromFile(const VertexShaderDescriptor& descriptor)
{
	this->vertexShaders.emplace_back(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor);

	return this->vertexShaders.size() - 1;
}

size_t Engine::loadPixelShaderFromFile(const PixelShaderDescriptor& descriptor)
{
	this->pixelShaders.emplace_back(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor);

	return this->pixelShaders.size() - 1;
}

size_t Engine::createConstantBuffer(const ConstantBufferDescriptor& descriptor)
{
	this->constantBuffers.emplace_back(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor);

	return this->constantBuffers.size() - 1;
}

Mesh Engine::createMeshFromVertices(const MeshDescriptorFromVertices& descriptor)
{
	Mesh mesh{
		VertexBuffer(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.vertexBufferDescriptor),
		IndexBuffer(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.indices),
		descriptor.vertexShaderIndex,
		descriptor.pixelShaderIndex,
		descriptor.constantBufferIndices
	};

	return mesh;
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

void Engine::drawMesh(Mesh& mesh)
{
	mesh.vb.Bind();
	mesh.ib.Bind();
	this->vertexShaders[mesh.vsIndex].Bind();
	this->pixelShaders[mesh.psIndex].Bind();

	for (const uint16_t cbIndex : mesh.cbIndices)
		this->constantBuffers[cbIndex].Bind();

	this->graphics->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->graphics->getDeviceContext()->DrawIndexed(static_cast<UINT>(mesh.ib.getSize()), 0u, 0u);
}

void Engine::update() { this->window->update(); }

void Engine::render(const bool useVSync) { this->graphics->render(useVSync); }

void Engine::run(const std::function<void(float)>& functor, const bool useVSync, const uint16_t fps) {
	Timer timer;
	uint32_t frames = 0;

	while (this->window->isRunning())
	{
		const uint32_t elapsed = timer.getElapsedTime();

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

		this->update();
		
		functor(elapsed);

		this->render(useVSync);
	}
}