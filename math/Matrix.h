#pragma once

#include "../common/Include.h"

template <typename T, size_t R, size_t C>
class Matrix
{
	T m[R][C];

	Matrix(const bool bIsIdentity = false)
	{
		if (bIsIdentity)
			for (size_t r = 0u; r < R; r++)
				for (size_t c = 0u; c < C; c++)
					this->m[r][c] = 1.f;
	}

	[[nodiscard]] size_t GetRowCount() const noexcept { return R; }
	[[nodiscard]] size_t GetColCount() const noexcept { return C; }

	T* operator[](const size_t r) noexcept { return &this->m[r]; }
	T& operator()(const size_t r, const size_t c) noexcept { return this->m[r][c]; }
};