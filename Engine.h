#pragma once

#include "Engine.h"

#include "math/Vectors.h"
#include "math/Vertex.h"

#include "misc/Constants.h"
#include "misc/Timer.h"

#include "winapi/audio/AudioHandler.h"

#include "winapi/graphics/buffers/ConstantBuffer.h"
#include "winapi/graphics/buffers/IndexBuffer.h"
#include "winapi/graphics/buffers/VertexBuffer.h"

#include "winapi/graphics/cameras/Camera.h"
#include "winapi/graphics/cameras/OrthographicCamera.h"
#include "winapi/graphics/cameras/PerspectiveCamera.h"

#include "winapi/graphics/Graphics.h"

#include "winapi/graphics/images/Image.h"
#include "winapi/graphics/images/Texture2D.h"
#include "winapi/graphics/images/TextureSampler.h"

#include "winapi/graphics/meshes/Mesh.h"

#include "winapi/graphics/misc/ShaderBindingType.h"

#include "winapi/graphics/shaders/PixelShader.h"
#include "winapi/graphics/shaders/VertexShader.h"

#include "winapi/misc/Error.h"
#include "winapi/misc/includes.h"

#include "winapi/windows/peripherals/Keyboard.h"
#include "winapi/windows/peripherals/Mouse.h"
#include "winapi/windows/peripherals/PeripheralDevice.h"

#include "winapi/windows/Window.h"

#include <thread>
#include <optional>

struct MeshDescriptorFromVertices
{
	const VertexBufferDescriptor& vertexBufferDescriptor;
	const std::vector<uint32_t>& indices;
	const size_t vertexShaderIndex;
	const size_t pixelShaderIndex;
	const std::optional<size_t> t2dIndex;
	const std::optional<size_t> tsIndex;
	const std::vector<size_t>& constantBufferIndices;
	const D3D_PRIMITIVE_TOPOLOGY primitiveTopology;
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

	std::vector<Mesh>           meshes;
	std::vector<Texture2D>      textures;
	std::vector<PixelShader>    pixelShaders;
	std::vector<VertexShader>   vertexShaders;
	std::vector<ConstantBuffer> constantBuffers;
	std::vector<TextureSampler> textureSamplers;

	Engine(const WindowDescriptor& windowDesc);

	virtual void onRender(const uint32_t elapsed) = 0;

	void captureCursor();

	size_t loadVertexShaderFromFile(const VertexShaderDescriptor& descriptor);

	size_t loadPixelShaderFromFile(const PixelShaderDescriptor& descriptor);

	size_t loadTextureFromImage(const Texture2DDescriptor& descriptor);

	size_t loadTextureSampler(const TextureSamplerDescriptor& descriptor);

	size_t createConstantBuffer(const ConstantBufferDescriptor& descriptor);

	size_t createMeshFromVertices(const MeshDescriptorFromVertices& descriptor);

	DataFromMeshFile loadDataFromMeshFile(const MeshDescriptorFromFile& descriptor);

	void drawMesh(const size_t mesh);

	void run(const bool useVSync = true, const uint16_t fps = 60);
};