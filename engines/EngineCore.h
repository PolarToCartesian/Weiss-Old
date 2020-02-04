#pragma once

#include "../misc/Window.h"
#include "../misc/Defines.h"

#include "../timers/Timer.h"

#include "../objects/Mesh.h"

#include "../buffers/IndexBuffer.h"
#include "../buffers/VertexBuffer.h"
#include "../buffers/ConstantBuffer.h"

#include "../shaders/PixelShader.h"
#include "../shaders/VertexShader.h"

#include "../media/Image.h"
#include "../media/Texture2D.h"
#include "../media/TextureSampler.h"

class EngineInitializationException : public std::exception { };

struct DataFromMeshFile
{
	std::vector<Vec3f> vertices;
	std::vector<uint32_t> indices;
};

class EngineCore
{
// --> ENGINE --> ENGINE CLASS --> CORE START
protected:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTarget;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_pBlendState;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOn;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOff;

	std::vector<Mesh>           meshes;

	std::vector<PixelShader>    pixelShaders;
	std::vector<VertexShader>   vertexShaders;

	std::vector<VertexBuffer>   vertexBuffers;
	std::vector<IndexBuffer>    indexBuffers;
	std::vector<ConstantBuffer> constantBuffers;

	std::vector<Texture2D>      textures;
	std::vector<TextureSampler> textureSamplers;

	size_t    windowIndex = 0u;
	Mouse*    mouse = nullptr;
	Keyboard* keyboard = nullptr;

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTarget();
	void CreateViewport();
	void CreateDepthStencilStates();
	void CreateDepthStencil();
	void BindDepthStencil();
	void CreateAndBindBlendState();

	/*
	 * This function initializes DirectX and creates key components
	*/
	void InitGraphics();

	/* 
	 * This functions swaps the back buffer and the front buffer to show the frame to the user
	 * It also clears the depthStencilView
	 */
	void PresentFrame(const bool useVSync);

	void CreateDefaultConstantBuffers();

public:
	EngineCore();

	~EngineCore();

	void InitEngineCore(const WindowDescriptor& windowDesc);

	void Run(const bool useVSync = true, const uint16_t fps = 60);

	virtual void OnRender(const float elapsed) = 0;

	void CaptureCursor();

	void PlayWavFile(const char* filename);

	void DrawMesh(const size_t meshIndex, UINT count = 0u);

	[[nodiscard]] size_t CreateVertexShader(const VertexShaderDescriptor& descriptor);

	[[nodiscard]] size_t CreatePixelShader(const PixelShaderDescriptor& descriptor);

	[[nodiscard]] size_t CreateTextureFromImage(const Texture2DDescriptor& descriptor);

	[[nodiscard]] size_t CreateTextureSampler(const TextureSamplerDescriptor& descriptor);

	[[nodiscard]] size_t CreateConstantBuffer(const ConstantBufferDescriptor& descriptor);

	[[nodiscard]] size_t CreateVertexBuffer(const VertexBufferDescriptor& descriptor);

	[[nodiscard]] size_t CreateIndexBuffer(const IndexBufferDescriptor& descriptor);

	[[nodiscard]] size_t CreateMeshFromVertices(const Mesh& mesh);

	[[nodiscard]] DataFromMeshFile LoadDataFromMeshFile(const char* filename);

	[[nodiscard]] Mesh&           GetMesh          (const size_t index) noexcept;
	[[nodiscard]] Texture2D&      GetTexture       (const size_t index) noexcept;
	[[nodiscard]] VertexShader&   GetVertexShader  (const size_t index) noexcept;
	[[nodiscard]] PixelShader&    GetPixelShader   (const size_t index) noexcept;
	[[nodiscard]] ConstantBuffer& GetConstantBuffer(const size_t index) noexcept;
	[[nodiscard]] VertexBuffer&   GetVertexBuffer  (const size_t index) noexcept;
	[[nodiscard]] IndexBuffer&    GetIndexBuffer   (const size_t index) noexcept;
	[[nodiscard]] TextureSampler& GetTextureSampler(const size_t index) noexcept;

	// Only Interact With A Window Through Its Index Because The "m_s_windows" Array Changes When New Windows Are Created
	[[nodiscard]] Window&   GetWindow()         noexcept;
	[[nodiscard]] Mouse&    GetMouse()          noexcept;
	[[nodiscard]] Keyboard& GetKeybaord()       noexcept;
	[[nodiscard]] EngineCore&   GetEngineCore() noexcept;
};