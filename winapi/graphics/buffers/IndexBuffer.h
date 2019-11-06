#pragma once

#include "../../misc/Error.h"
#include "../../../math/Vertex.h"

#include <wrl.h>
#include <vector>
#include <d3d11.h>

class IndexBuffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		
	public:
		std::vector<unsigned short> indices;

	public:
		IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, const std::vector<unsigned short>& indices);

		void Bind(Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef) const noexcept;
};