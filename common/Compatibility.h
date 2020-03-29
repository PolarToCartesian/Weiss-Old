#pragma once

#include "Pch.h"

struct Rect { // basicly Window's RECT
	uint16_t left   = 0u;
	uint16_t top    = 0u;
	uint16_t right  = 0u;
	uint16_t bottom = 0u;

	Rect() { }

#ifdef __WEISS__OS_WINDOWS
	Rect(const RECT& rect)
	{
		this->left = static_cast<uint16_t>(rect.left);
		this->top = static_cast<uint16_t>(rect.top);
		this->right = static_cast<uint16_t>(rect.right);
		this->bottom = static_cast<uint16_t>(rect.bottom);
	}

	operator RECT() const noexcept { return RECT{ left, top, right, bottom }; }
#endif // __WEISS__OS_WINDOWS
};