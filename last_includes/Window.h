#pragma once

std::list<Window> Window::m_s_windows = std::list<Window>();

LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	#ifdef _WIN64
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	#else
		Window* window = (Window*)GetWindowLong(hwnd, GWLP_USERDATA);
	#endif

	if (window != NULL)
		return window->HandleMessage(msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}