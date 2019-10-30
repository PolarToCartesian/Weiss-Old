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

// Operator |+=|-=|*=|/=|

template <typename T, size_t S, typename T2>
void operator+=(std::array<T, S>& v, const T2 n) { for (size_t i = 0; i < S; i++) v[i] += n; }

template <typename T, size_t S, typename T2>
void operator-=(std::array<T, S>& v, const T2 n) { for (size_t i = 0; i < S; i++) v[i] -= n; }

template <typename T, size_t S, typename T2>
void operator*=(std::array<T, S>& v, const T2 n) { for (size_t i = 0; i < S; i++) v[i] *= n; }

template <typename T, size_t S, typename T2>
void operator/=(std::array<T, S>& v, const T2 n) { for (size_t i = 0; i < S; i++) v[i] /= n; }

template <typename T, size_t S1, size_t S2>
void operator+=(std::array<T, S1>& a, const std::array<T, S2>& b) { for (size_t i = 0; i < std::min(S1, S2); i++) a[i] += b[i]; }

template <typename T, size_t S1, size_t S2>
void operator-=(std::array<T, S1>& a, const std::array<T, S2>& b) { for (size_t i = 0; i < std::min(S1, S2); i++) a[i] -= b[i]; }

template <typename T, size_t S1, size_t S2>
void operator*=(std::array<T, S1>& a, const std::array<T, S2>& b) { for (size_t i = 0; i < std::min(S1, S2); i++) a[i] *= b[i]; }

template <typename T, size_t S1, size_t S2>
void operator/=(std::array<T, S1>& a, const std::array<T, S2>& b) { for (size_t i = 0; i < std::min(S1, S2); i++) a[i] /= b[i]; }

// Operator |+|-|*|/|

template <typename T1, size_t S, typename T2>
std::array<T1, S> operator+(const std::array<T1, S>& a, const T2 n) { std::array<T1, S> result(a); result += n; return result; }

template <typename T1, size_t S, typename T2>
std::array<T1, S> operator-(const std::array<T1, S>& a, const T2 n) { std::array<T1, S> result(a); result -= n; return result; }

template <typename T1, size_t S, typename T2>
std::array<T1, S> operator*(const std::array<T1, S>& a, const T2 n) { std::array<T1, S> result(a); result *= n; return result; }

template <typename T1, size_t S, typename T2>
std::array<T1, S> operator/(const std::array<T1, S>& a, const T2 n) { std::array<T1, S> result(a); result /= n; return result; }

template <typename T, size_t S1, size_t S2>
std::array<T, S1> operator+(const std::array<T, S1>& a, const std::array<T, S2>& b) { std::array<T, S1> result(a); result += b; return result; }

template <typename T, size_t S1, size_t S2>
std::array<T, S1> operator-(const std::array<T, S1>& a, const std::array<T, S2>& b) { std::array<T, S1> result(a); result -= b; return result; }

template <typename T, size_t S1, size_t S2>
std::array<T, S1> operator*(const std::array<T, S1>& a, const std::array<T, S2>& b) { std::array<T, S1> result(a); result *= b; return result; }

template <typename T, size_t S1, size_t S2>
std::array<T, S1> operator/(const std::array<T, S1>& a, const std::array<T, S2>& b) { std::array<T, S1> result(a); result /= b; return result; }

// Printing

template <typename T, size_t S>
std::ostream& operator<<(std::ostream& stream, std::array<T, S>& v) {
	stream << '(';

	for (auto it = v.begin(); it != v.end() - 1; it++)
		stream << *it << ", ";

	return stream << v[S - 1] << ')';
}