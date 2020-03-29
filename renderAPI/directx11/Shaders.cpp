#include "Shaders.h"

#ifdef __WEISS__OS_WINDOWS

DirectX11VertexShader::DirectX11VertexShader(RenderAPI* pRenderAPI, const char* sourceFilename, const std::vector<ShaderInputElement>& sies)
	: m_pRenderAPI(pRenderAPI)
{
	DirectX11RenderAPI* d3d11RenderAPI = reinterpret_cast<DirectX11RenderAPI*>(this->m_pRenderAPI);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	// Create Shader
	std::ifstream fileStream(sourceFilename);
	std::string   sourceCode((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

	if (D3DCompile(sourceCode.c_str(), sourceCode.size(), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &pBlob, NULL) != S_OK)
		throw std::runtime_error("Could Not Compile Vertex Shader");

	if (d3d11RenderAPI->GetDevice()->GetDevice()->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pShader) != S_OK)
		throw std::runtime_error("Could Not Create Vertex Shader");

	// Create Input Layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptors(sies.size());
	for (uint32_t i = 0; i < inputElementDescriptors.size(); i++)
	{
		inputElementDescriptors[i] = D3D11_INPUT_ELEMENT_DESC{};
		inputElementDescriptors[i].SemanticName = sies[i].name;

		switch (sies[i].type)
		{
		case ShaderInputElementType::FLOAT_32:
			inputElementDescriptors[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
			break;
		case ShaderInputElementType::UINT_32:
			inputElementDescriptors[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
			break;
		case ShaderInputElementType::VECTOR_2D_FLOAT_32:
			inputElementDescriptors[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
			break;
		case ShaderInputElementType::VECTOR_3D_FLOAT_32:
			inputElementDescriptors[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case ShaderInputElementType::VECTOR_4D_FLOAT_32:
			inputElementDescriptors[i].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		default:
			throw std::runtime_error("Input Element Type Not Supported");
		}

		inputElementDescriptors[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		inputElementDescriptors[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	}

	if (d3d11RenderAPI->GetDevice()->GetDevice()->CreateInputLayout(inputElementDescriptors.data(), (UINT)inputElementDescriptors.size(), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &this->m_pInputLayout) != S_OK)
		throw std::runtime_error("Could Not Create Input Layout");
}

void DirectX11VertexShader::Bind()
{
	DirectX11RenderAPI* d3d11RenderAPI = reinterpret_cast<DirectX11RenderAPI*>(this->m_pRenderAPI);

	d3d11RenderAPI->GetDevice()->GetDeviceContext()->IASetInputLayout(this->m_pInputLayout.Get());
	d3d11RenderAPI->GetDevice()->GetDeviceContext()->VSSetShader(this->m_pShader.Get(), nullptr, 0u);
}

DirectX11PixelShader::DirectX11PixelShader(RenderAPI* pRenderAPI, const char* sourceFilename)
	: m_pRenderAPI(pRenderAPI)
{
	DirectX11RenderAPI* d3d11RenderAPI = reinterpret_cast<DirectX11RenderAPI*>(this->m_pRenderAPI);

	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	std::ifstream fileStream(sourceFilename);
	std::string sourceCode((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

	if (D3DCompile(sourceCode.c_str(), sourceCode.size(), NULL, NULL, NULL, "main", "ps_5_0", 0, 0, &pBlob, NULL) != S_OK)
		throw std::runtime_error("Could Not Compile Pixel Shader");

	if (d3d11RenderAPI->GetDevice()->GetDevice()->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &this->m_pShader) != S_OK)
		throw std::runtime_error("Could Not Create Pixel Shader");
}

void DirectX11PixelShader::Bind()
{
	DirectX11RenderAPI* d3d11RenderAPI = reinterpret_cast<DirectX11RenderAPI*>(this->m_pRenderAPI);

	d3d11RenderAPI->GetDevice()->GetDeviceContext()->PSSetShader(this->m_pShader.Get(), nullptr, 0u);
}

#endif // __WEISS__OS_WINDOWS