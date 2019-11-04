#pragma once

#include <wrl.h>
#include <d3d11.h>

class Buffer {
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pBuffer;

	public:
		virtual void Bind() = 0;
};