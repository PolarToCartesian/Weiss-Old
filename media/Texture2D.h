#include "Image.h"
#include "../misc/ShaderBindingLoading.h"

class Texture2DCreationException : public std::exception { };

struct Texture2DDescriptor
{
	ShaderBindingType bindingType;
	std::vector<Image> images;
	UINT slotVS;
	UINT slotPS;
	bool useMipMaps;
};

class Texture2D {
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>&     m_pDeviceContextRef;

	Texture2DDescriptor m_descriptor;

public:
	Texture2D(const Microsoft::WRL::ComPtr<ID3D11Device>&		 pDeviceRef,
					Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
			  const Texture2DDescriptor&						 descriptor);

	void Bind() const noexcept;
};