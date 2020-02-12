#pragma once

#include "../window/Window.h"
#include "../misc/DeviceInfo.h"

class LowLevelGraphicsInitializerException : public std::exception { };

class LowLevelGraphicsInitializer
{
protected:
	Microsoft::WRL::ComPtr<ID3D11Device>            m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain>          m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11BlendState>        m_pBlendState;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_pDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_pRenderTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_pDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOn;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencilStateForZBufferOff;

private:
	void CreateDeviceAndSwapChain(Window* window);
	void CreateRenderTarget();
	void CreateViewport(Window* window);
	void CreateDepthStencilStates();
	void CreateDepthStencil(Window* window);
	void CreateAndBindBlendState();

protected:
	void InitializeLowLevelGraphics(Window* window);

	DeviceInfo GetDeviceInfo() noexcept;
};