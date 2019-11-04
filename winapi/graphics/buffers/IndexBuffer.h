#pragma once

#include "../../misc/Error.h"
#include "../../../math/Vertex.h"

#include <wrl.h>
#include <vector>
#include <d3d11.h>

class IndexBuffer {
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		std::vector<unsigned short> indices;

	public:
		IndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<unsigned short>& indices);

		void Bind() const noexcept;
};