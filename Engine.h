#pragma once

#include "winapi/graphics/shaders/PixelShader.h"
#include "winapi/graphics/shaders/VertexShader.h"

#include "winapi/graphics/buffers/ConstantBuffer.h"

#include "winapi/window/Window.h"

#include <optional>

struct MeshDescriptorFromVertices
{
	const std::vector<Vertex>& vertices;
	const std::vector<uint32_t>& indices;
	const uint16_t vertexShaderIndex;
	const uint16_t pixelShaderIndex;
	const std::vector<uint16_t>& constantBufferIndices;
};

struct MeshDescriptorFromFile
{
	const char* filename;
	const uint16_t vertexShaderIndex;
	const uint16_t pixelShaderIndex;
	const std::vector<uint16_t>& constantBufferIndices;
};

class Engine
{
	public:
		Window* window;
		Mouse* mouse;
		Keyboard* keyboard;

		std::unique_ptr<Graphics> graphics;

		std::vector<PixelShader>    pixelShaders;
		std::vector<VertexShader>   vertexShaders;
		std::vector<ConstantBuffer> constantBuffers;

		Engine(const WindowDescriptor& windowDesc)
		{
			this->window   = WindowManager::createWindow(windowDesc);
			this->graphics = this->window->createGraphics();
			
			this->mouse    = &(this->window->getMouse());
			this->keyboard = &(this->window->getKeyboard());
		}

		void bindCursor() {
			const uint16_t leftX = this->window->getPositionX() + 1;
			const uint16_t topY  = this->window->getPositionY() + 1;

			const uint16_t rightX  = this->window->getPositionX() + this->window->getDimensionW() - 1;
			const uint16_t bottomY = this->window->getPositionY() + this->window->getDimensionH() - 1;

			this->window->getMouse().clip(leftX, rightX, topY, bottomY);
		}

		uint16_t loadVertexShaderFromFile(const VertexShaderDescriptor& descriptor)
		{
			this->vertexShaders.emplace_back(VertexShader(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor));
			
			return this->vertexShaders.size() - 1;
		}

		uint16_t loadPixelShaderFromFile(const PixelShaderDescriptor& descriptor)
		{
			this->pixelShaders.emplace_back(PixelShader(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor));

			return this->pixelShaders.size() - 1;
		}

		uint16_t createConstantBuffer(const ConstantBufferDescriptor& descriptor)
		{
			this->constantBuffers.emplace_back(ConstantBuffer(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor));

			return this->constantBuffers.size() - 1;
		}

		Mesh createMeshFromVertices(const MeshDescriptorFromVertices& descriptor)
		{
			Mesh mesh{
				VertexBuffer(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.vertices),
				IndexBuffer (this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.indices),
				descriptor.vertexShaderIndex,
				descriptor.pixelShaderIndex,
				descriptor.constantBufferIndices
			};

			return mesh;
		}

		Mesh loadMeshFromFile(const MeshDescriptorFromFile& descriptor)
		{
			std::ifstream infile(descriptor.filename);

			std::vector<Vertex>   vertices;
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
					Vertex vertex{
						{0.f, 0.f, 0.f},
						{255, 255, 255, 255}
					};

					iss >> vertex.pos.x >> vertex.pos.y >> vertex.pos.z;

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

			std::cout << indices.size() / 3.f << '\n';

			const MeshDescriptorFromVertices desciptor2 = {
				vertices, indices, 
				descriptor.vertexShaderIndex, descriptor.pixelShaderIndex, descriptor.constantBufferIndices
			};

			return this->createMeshFromVertices(desciptor2);
		}

		void drawMesh(Mesh& mesh)
		{
			mesh.vb.Bind();
			mesh.ib.Bind();
			this->vertexShaders[mesh.vsIndex].Bind();
			this->pixelShaders[mesh.psIndex].Bind();

			for (const uint16_t cbIndex : mesh.cbIndices)
				this->constantBuffers[cbIndex].Bind();
			
			this->graphics->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			this->graphics->getDeviceContext()->DrawIndexed(mesh.ib.getSize(), 0u, 0u);
		}

		void update()
		{
			this->window->update();
		}

		void render(const bool useVSync)
		{
			this->graphics->render(useVSync);
		}
};