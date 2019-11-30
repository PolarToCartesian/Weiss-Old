#pragma once

#include "../misc/Includes.h"

typedef std::array<float,    2> Vec2f;
typedef std::array<int8_t,   2> Vec2i8;
typedef std::array<int16_t,  2> Vec2i16;
typedef std::array<int32_t,  2> Vec2i32;
typedef std::array<int64_t,  2> Vec2i64;
typedef std::array<uint8_t,  2> Vec2u8;
typedef std::array<uint16_t, 2> Vec2u16;
typedef std::array<uint32_t, 2> Vec2u32;
typedef std::array<uint64_t, 2> Vec2u64;

typedef std::array<float,    3> Vec3f;
typedef std::array<int8_t,   3> Vec3i8;
typedef std::array<int16_t,  3> Vec3i16;
typedef std::array<int32_t,  3> Vec3i32;
typedef std::array<int64_t,  3> Vec3i64;
typedef std::array<uint8_t,  3> Vec3u8;
typedef std::array<uint16_t, 3> Vec3u16;
typedef std::array<uint32_t, 3> Vec3u32;
typedef std::array<uint64_t, 3> Vec3u64;
typedef std::array<float,    3> Colorf;
typedef std::array<uint8_t,  3> Coloru;

typedef std::array<float,    4> Vec4f;
typedef std::array<int8_t,   4> Vec4i8;
typedef std::array<int16_t,  4> Vec4i16;
typedef std::array<int32_t,  4> Vec4i32;
typedef std::array<int64_t,  4> Vec4i64;
typedef std::array<uint8_t,  4> Vec4u8;
typedef std::array<uint16_t, 4> Vec4u16;
typedef std::array<uint32_t, 4> Vec4u32;
typedef std::array<uint64_t, 4> Vec4u64;
typedef std::array<uint8_t,  4> Vec4u8;
typedef std::array<int16_t,  4> Vec4i;

typedef Vec4f Color;

// Operator Overloading

template <typename T, size_t S>
std::array<T, S> operator+(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] += b[i]; return result; }
template <typename T, size_t S>
std::array<T, S> operator-(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] -= b[i]; return result; }
template <typename T, size_t S>
std::array<T, S> operator*(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] *= b[i]; return result; }
template <typename T, size_t S>
std::array<T, S> operator/(const std::array<T, S>& a, const std::array<T, S>& b) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] /= b[i]; return result; }

template <typename T, size_t S>
std::array<T, S> operator/(const std::array<T, S>& a, const float n) { std::array<T, S> result(a); for (size_t i = 0; i < S; i++) result[i] /= n; return result; }


template <typename T, size_t S>
void operator+=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] += b[i]; }
template <typename T, size_t S>
void operator-=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] -= b[i]; }
template <typename T, size_t S>
void operator*=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] *= b[i]; }
template <typename T, size_t S>
void operator/=(std::array<T, S>& a, const std::array<T, S>& b) { for (size_t i = 0; i < S; i++) a[i] /= b[i]; }

template <typename T, size_t S>
void operator+=(std::array<T, S>& a, const float n) { for (size_t i = 0; i < S; i++) a[i] += n; }
template <typename T, size_t S>
void operator-=(std::array<T, S>& a, const float n) { for (size_t i = 0; i < S; i++) a[i] -= n; }
template <typename T, size_t S>
void operator*=(std::array<T, S>& a, const float n) { for (size_t i = 0; i < S; i++) a[i] *= n; }
template <typename T, size_t S>
void operator/=(std::array<T, S>& a, const float n) { for (size_t i = 0; i < S; i++) a[i] /= n; }

// Printing

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& stream, const std::array<T, S>& v) {
	stream << '(';

	for (auto it = v.begin(); it != v.end() - 1; it++)
		stream << *it << ", ";

	return stream << v[S - 1] << ')';
}