#pragma once

std::vector<Window> Window::m_s_windows = std::vector<Window>();

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::vector<Window>::iterator it = std::find_if(Window::m_s_windows.begin(), Window::m_s_windows.end(), [&hwnd](const Window& window)
		{
			return window.m_handle == hwnd;
		});

	if (it != Window::m_s_windows.end())
		return it->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}