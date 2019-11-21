#pragma once

#include "winapi/graphics/shaders/PixelShader.h"
#include "winapi/graphics/shaders/VertexShader.h"

#include "winapi/graphics/buffers/ConstantBuffer.h"

#include "winapi/windows/Window.h"

#include <optional>

struct MeshDescriptorFromVertices
{
	const VertexBufferDescriptor& vertexBufferDescriptor;
	const std::vector<uint32_t>& indices;
	const uint16_t vertexShaderIndex;
	const uint16_t pixelShaderIndex;
	const std::vector<uint16_t>& constantBufferIndices;
};

struct MeshDescriptorFromFile
{
	const char* filename;
};

struct DataFromMeshFile
{
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
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

		Engine(const WindowDescriptor& windowDesc);

		void bindCursor();

		size_t loadVertexShaderFromFile(const VertexShaderDescriptor& descriptor);

		size_t loadPixelShaderFromFile(const PixelShaderDescriptor& descriptor);

		size_t createConstantBuffer(const ConstantBufferDescriptor& descriptor);

		Mesh createMeshFromVertices(const MeshDescriptorFromVertices& descriptor);

		DataFromMeshFile loadDataFromMeshFile(const MeshDescriptorFromFile& descriptor);

		void drawMesh(Mesh& mesh);

		void update();

		void render(const bool useVSync);
};