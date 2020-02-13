#pragma once

#include "../misc/Pch.h"

template <typename T>
struct Vector2D
{
	T x;
	T y;
	
	template <typename K>
	void operator+=(const Vector2D<K>& v) { this->x += v.x; this->y += v.y; }

	template <typename K>
	void operator-=(const Vector2D<K>& v) { this->x -= v.x; this->y -= v.y; }

	template <typename K>
	void operator*=(const Vector2D<K>& v) { this->x *= v.x; this->y *= v.y; }

	template <typename K>
	void operator/=(const Vector2D<K>& v) { this->x /= v.x; this->y /= v.y; }

	template <typename K>
	void operator+=(const K& n) { this->x += n; this->y += n; }

	template <typename K>
	void operator-=(const K& n) { this->x -= n; this->y -= n; }

	template <typename K>
	void operator*=(const K& n) { this->x *= n; this->y *= n; }

	template <typename K>
	void operator/=(const K& n) { this->x /= n; this->y /= n; }

	template<typename K>
	bool operator==(const Vector2D<K>& v) { return this->x == v.x && this->y == v.y; }

	template<typename K>
	bool operator!=(const Vector2D<K>& v) { return this->x != v.x || this->y != v.y; }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector2D<T>& v)
{
	os << '(' << v.x << ", " << v.y << ")";
	return os;
}

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator+(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x + b.x, a.y + b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator-(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x - b.x, a.y - b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator*(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x * b.x, a.y * b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator/(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x / b.x, a.y / b.y }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator+(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x + n, v.y + n }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator-(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x - n, v.y - n }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator*(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x * n, v.y * n }; }

template <typename T, typename K>
[[nodiscard]] Vector2D<T> operator/(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x / n, v.y / n }; }

template <typename T>
struct Vector3D : Vector2D<T>
{
	T z;

	template <typename K>
	void operator+=(const Vector3D<K>& v) { this->x += v.x; this->y += v.y; this->z += v.z; }

	template <typename K>
	void operator-=(const Vector3D<K>& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; }

	template <typename K>
	void operator*=(const Vector3D<K>& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; }

	template <typename K>
	void operator/=(const Vector3D<K>& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; }

	template <typename K>
	void operator+=(const K& n) { this->x += n; this->y += n; this->z += n; }

	template <typename K>
	void operator-=(const K& n) { this->x -= n; this->y -= n; this->z -= n; }

	template <typename K>
	void operator*=(const K& n) { this->x *= n; this->y *= n; this->z *= n; }

	template <typename K>
	void operator/=(const K& n) { this->x /= n; this->y /= n; this->z /= n; }

	template<typename K>
	bool operator==(const Vector3D<K>& v) { return this->x == v.x && this->y == v.y && this->z == v.z; }

	template<typename K>
	bool operator!=(const Vector3D<K>& v) { return this->x != v.x || this->y != v.y || this->z != v.z; }
};

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator+(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x + b.x, a.y + b.y, a.z + b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator-(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x - b.x, a.y - b.y, a.z - b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator*(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x * b.x, a.y * b.y, a.z * b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator/(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x / b.x, a.y / b.y, a.z / b.z }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator+(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x + n, v.y + n, v.z + n }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator-(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x - n, v.y - n, v.z - n }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator*(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x * n, v.y * n, v.z * n }; }

template <typename T, typename K>
[[nodiscard]] Vector3D<T> operator/(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x / n, v.y / n, v.z / n }; }

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

typedef Vector2D<float>    Vec2f;
typedef Vector2D<int8_t>   Vec2i8;
typedef Vector2D<int16_t>  Vec2i16;
typedef Vector2D<int32_t>  Vec2i32;
typedef Vector2D<int64_t>  Vec2i64;
typedef Vector2D<uint8_t>  Vec2u8;
typedef Vector2D<uint16_t> Vec2u16;
typedef Vector2D<uint32_t> Vec2u32;
typedef Vector2D<uint64_t> Vec2u64;

typedef Vector3D<float>    Vec3f;
typedef Vector3D<int8_t>   Vec3i8;
typedef Vector3D<int16_t>  Vec3i16;
typedef Vector3D<int32_t>  Vec3i32;
typedef Vector3D<int64_t>  Vec3i64;
typedef Vector3D<uint8_t>  Vec3u8;
typedef Vector3D<uint16_t> Vec3u16;
typedef Vector3D<uint32_t> Vec3u32;
typedef Vector3D<uint64_t> Vec3u64;