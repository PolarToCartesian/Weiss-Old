#pragma once

#include "Engine.h"

#include "math/Constants.h"
#include "math/Conversions.h"
#include "math/Vectors.h"
#include "math/Misc.h"

#include "Mesh.h"

#include "misc/Timer.h"

#include "audio/AudioHandler.h"

#include "buffers/ConstantBuffer.h"
#include "buffers/IndexBuffer.h"
#include "buffers/VertexBuffer.h"

#include "cameras/Camera.h"
#include "cameras/OrthographicCamera.h"
#include "cameras/PerspectiveCamera.h"

#include "images/Image.h"
#include "images/Texture2D.h"
#include "images/TextureSampler.h"

#include "misc/ShaderBindingType.h"

#include "shaders/PixelShader.h"
#include "shaders/VertexShader.h"

#include "misc/Includes.h"

#include "peripherals/Keyboard.h"
#include "peripherals/Mouse.h"
#include "peripherals/PeripheralDevice.h"

#include "Window.h"

struct MeshDescriptorFromVertices
{
	const VertexBufferDescriptor vertexBufferDescriptor;
	const std::vector<uint32_t> indices;
	const size_t vertexShaderIndex;
	const size_t pixelShaderIndex;
	const std::vector<size_t> t2dIndices;
	const std::optional<size_t> tsIndex;
	const std::vector<size_t> constantBufferIndices;
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
private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

public:
	Window* window;
	Mouse* mouse;
	Keyboard* keyboard;

	std::vector<size_t> renderPool;

	std::vector<Mesh>           meshes;
	std::vector<Texture2D>      textures;
	std::vector<PixelShader>    pixelShaders;
	std::vector<VertexShader>   vertexShaders;
	std::vector<ConstantBuffer> constantBuffers;
	std::vector<TextureSampler> textureSamplers;

private:
	void createDeviceAndSwapChain();

	void createRenderTarget();

	void createViewport();

	void createDepthBuffer();

	void initGraphics();

	void drawMesh(const size_t mesh);

	void presentFrame(const bool useVSync);

public:
	Engine();

	void createWindow(const WindowDescriptor& windowDesc);

	virtual void onRender(const float elapsed) = 0;

	void captureCursor();

	size_t loadVertexShaderFromFile(const VertexShaderDescriptor& descriptor);

	size_t loadPixelShaderFromFile(const PixelShaderDescriptor& descriptor);

	size_t loadTextureFromImage(const Texture2DDescriptor& descriptor);

	size_t loadTextureSampler(const TextureSamplerDescriptor& descriptor);

	size_t createConstantBuffer(const ConstantBufferDescriptor& descriptor);

	size_t createMeshFromVertices(const MeshDescriptorFromVertices& descriptor);

	DataFromMeshFile loadDataFromMeshFile(const MeshDescriptorFromFile& descriptor);

	void queueMeshForRendering(const size_t mesh);

	void fill(const Color& color);

	void run(const bool useVSync = true, const uint16_t fps = 60);
};