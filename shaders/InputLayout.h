#pragma once

#include "../misc/Pch.h"
#include "../misc/DeviceInfo.h"

class InputLayoutCreationException : public std::exception { };

class InputLayout {
private:
	const DeviceInfo& m_deviceInfo;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;

public:
	InputLayout(const DeviceInfo& deviceInfo, const std::vector<std::pair<const char*, DXGI_FORMAT>>& ieds, const Microsoft::WRL::ComPtr<ID3DBlob>& pBlob);

	void Bind() const noexcept;
};