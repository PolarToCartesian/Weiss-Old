#pragma once

#include "../common/Include.h"

struct Coloru8
{
	uint8_t red   = 0u;
	uint8_t green = 0u;
	uint8_t blue  = 0u;
	uint8_t alpha = 0u;
};

constexpr const Coloru8 COLOR_U8_RED   = Coloru8{ 255u, 0u,   0u,   255u };
constexpr const Coloru8 COLOR_U8_GREEN = Coloru8{ 0u,   255u, 0u,   255u };
constexpr const Coloru8 COLOR_U8_BLUE  = Coloru8{ 0u,   0u,   255u, 255u };
constexpr const Coloru8 COLOR_U8_WHITE = Coloru8{ 255u, 255u, 255u, 255u };
constexpr const Coloru8 COLOR_U8_BLACK = Coloru8{ 0u,   0u,   0u,   255u };

struct Colorf32
{
	float red;
	float green;
	float blue;
	float alpha;
};

constexpr const Colorf32 COLOR_F32_RED   = Colorf32{ 1.f, 0.f, 0.f, 1.f };
constexpr const Colorf32 COLOR_F32_GREEN = Colorf32{ 0.f, 1.f, 0.f, 1.f };
constexpr const Colorf32 COLOR_F32_BLUE  = Colorf32{ 0.f, 0.f, 1.f, 1.f };
constexpr const Colorf32 COLOR_F32_WHITE = Colorf32{ 1.f, 1.f, 1.f, 1.f };
constexpr const Colorf32 COLOR_F32_BLACK = Colorf32{ 0.f, 0.f, 0.f, 1.f };