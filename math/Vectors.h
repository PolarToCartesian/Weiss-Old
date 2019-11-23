#pragma once

#include <array>
#include <iostream>

typedef std::array<float,    2> Vec2f;
typedef std::array<uint16_t, 2> Vec2u;
typedef std::array<int16_t,  2> Vec2i;

typedef std::array<float,    3> Vec3f;
typedef std::array<uint16_t, 3> Vec3u;
typedef std::array<uint8_t,  3> Vec3u8;
typedef std::array<int16_t,  3> Vec3i;
typedef std::array<float,    3> Colorf;
typedef std::array<uint8_t,  3> Coloru;

typedef std::array<float,    4> Vec4f;
typedef std::array<uint16_t, 4> Vec4u;
typedef std::array<uint8_t,  4> Vec4u8;
typedef std::array<int16_t,  4> Vec4i;

typedef Vec4f Color;

// Operator Overloading

template <typename T, size_t S>
std::array<T, S> operator+(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] += b[i]; }
template <typename T, size_t S>
std::array<T, S> operator-(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] -= b[i]; }
template <typename T, size_t S>
std::array<T, S> operator*(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] *= b[i]; }
template <typename T, size_t S>
std::array<T, S> operator/(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] /= b[i]; }

template <typename T, size_t S>
void operator+=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] += b[i]; }
template <typename T, size_t S>
void operator-=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] -= b[i]; }
template <typename T, size_t S>
void operator*=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] *= b[i]; }
template <typename T, size_t S>
void operator/=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] /= b[i]; }

// Printing

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& stream, const std::array<T, S>& v) {
	stream << '(';

	for (auto it = v.begin(); it != v.end() - 1; it++)
		stream << *it << ", ";

	return stream << v[S - 1] << ')';
}