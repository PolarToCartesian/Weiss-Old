#pragma once

#include <array>
#include <iostream>

typedef std::array<float,    2> Vec2f;
typedef std::array<uint16_t, 2> Vec2u;
typedef std::array<int16_t,  2> Vec2i;

typedef std::array<float,    3> Vec3f;
typedef std::array<uint16_t, 3> Vec3u;
typedef std::array<int16_t,  3> Vec3i;
typedef std::array<float,    3> Colorf;
typedef std::array<uint8_t,  3> Coloru;

typedef std::array<float,    4> Vec4f;
typedef std::array<uint16_t, 4> Vec4u;
typedef std::array<int16_t,  4> Vec4i;

typedef Vec4f Color;

// Printing

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& stream, const std::array<T, S>& v) {
	stream << '(';

	for (auto it = v.begin(); it != v.end() - 1; it++)
		stream << *it << ", ";

	return stream << v[S - 1] << ')';
}