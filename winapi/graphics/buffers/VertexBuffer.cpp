#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Device>& pDeviceRef, Microsoft::WRL::ComPtr<ID3D11DeviceContext>& pDeviceContextRef, const std::vector<Vertex>& vertices)
	: m_pDeviceContextRef(pDeviceContextRef), nVertices(vertices.size())
{
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(Vertex) * this->nVertices;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = &vertices[0];
	pDeviceRef->CreateBuffer(&bd, &sd, &this->m_pBuffer);
}

void VertexBuffer::Bind()
{
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	this->m_pDeviceContextRef->IASetVertexBuffers(0u, 1u, this->m_pBuffer.GetAddressOf(), &stride, &offset);
}