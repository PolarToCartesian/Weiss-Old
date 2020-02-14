#include "VertexShader.h"

VertexShader::VertexShader(const DeviceInfo& deviceInfo, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const char* sourceFilename)
	: Shader<ID3D11VertexShader>(deviceInfo, sourceFilename)
{
	this->Load();
	this->m_inputLayout = new InputLayout(deviceInfo, ieds, this->m_pBlob);
}

VertexShader::~VertexShader()
{
	delete this->m_inputLayout;
}

void VertexShader::Load()
{
	std::ifstream fileStream(this->m_sourceFilename);
	std::string sourceCode((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));

	if (D3DCompile(sourceCode.c_str(), sourceCode.size(), NULL, NULL, NULL, "main", "vs_5_0", 0, 0, &this->m_pBlob, NULL) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Compile Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw VertexShaderCreationException();
	}

	if (this->m_deviceInfo.m_pDevice->CreateVertexShader(this->m_pBlob->GetBufferPointer(), this->m_pBlob->GetBufferSize(), nullptr, &this->m_pResource) != S_OK)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("Could Not Create Pixel Shader");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw VertexShaderCreationException();
	}
}

void VertexShader::Bind() const noexcept
{
	this->m_inputLayout->Bind();
	this->m_deviceInfo.m_pDeviceContext->VSSetShader(this->m_pResource.Get(), nullptr, 0u);
}