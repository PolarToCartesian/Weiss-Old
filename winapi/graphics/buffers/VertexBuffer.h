#pragma once

#include "../../misc/Error.h"
#include "../../../math/Vertex.h"

#include <wrl.h>
#include <vector>
#include <d3d11.h>

class VertexBuffer {
	private:
		size_t nVertices;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContextRef;

	public:
		VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, 
						   Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef,
					 const std::vector<Vertex>& vertices);

		void Bind() const noexcept;
};