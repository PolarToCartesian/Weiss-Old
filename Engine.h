#pragma once

#include "winapi/window/Window.h"

#include <optional>

struct MeshDescriptorFromVertices
{
	const std::vector<Vertex>& vertices;
	const std::vector<uint32_t>& indices;
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ieds;
	const wchar_t* vertexShaderFilename;
	const wchar_t* pixelShaderFilename;
	const std::vector<ConstantBufferDescriptor>& constantBufferDescriptors;
};

struct MeshDescriptorFromFile
{
	const char* filename;
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ieds;
	const wchar_t* vertexShaderFilename;
	const wchar_t* pixelShaderFilename;
	const std::vector<ConstantBufferDescriptor>& constantBufferDescriptors;
};

class Engine
{
	public:
		Window* window;
		Mouse* mouse;
		Keyboard* keyboard;

		std::unique_ptr<Graphics> graphics;

		std::vector<PixelShader>  pixelShaders;
		std::vector<VertexShader> vertexShaders;

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

		Mesh createMeshFromVertices(const MeshDescriptorFromVertices& descriptor)
		{
			Mesh mesh{
				VertexBuffer(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.vertices),
				IndexBuffer (this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.indices),
				VertexShader(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.ieds, descriptor.vertexShaderFilename),
				PixelShader (this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor.pixelShaderFilename),
				{}
			};

			for (const ConstantBufferDescriptor& descriptor : descriptor.constantBufferDescriptors)
				mesh.cbs.emplace_back(this->graphics->getDevice(), this->graphics->getDeviceContext(), descriptor);

			return mesh;
		}

		Mesh loadMeshFromFile(const MeshDescriptorFromFile& descriptor)
		{
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			std::ifstream infile(descriptor.filename);

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

			return this->createMeshFromVertices({
				vertices, indices, descriptor.ieds, descriptor.vertexShaderFilename, 
				descriptor.pixelShaderFilename, descriptor.constantBufferDescriptors
			});
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