#pragma once

#include "../misc/Pch.h"

struct DeviceInfo
{
	Microsoft::WRL::ComPtr<ID3D11Device>& m_pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& m_pDeviceContext;
};