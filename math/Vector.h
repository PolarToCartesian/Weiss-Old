#pragma once

#include "../common/Include.h"

template <typename T>
struct Vector2D
{
	T x;
	T y;

	Vector2D() {}

	template <typename K>
	Vector2D(const K& n) : x(static_cast<T>(n)), y(static_cast<T>(n)) {}

	template <typename K, typename L>
	Vector2D(const K& x, const L& y) : x(static_cast<T>(x)), y(static_cast<T>(y)) {}
	
	template <typename K>
	inline void operator+=(const Vector2D<K>& v) { this->x += v.x; this->y += v.y; }

	template <typename K>
	inline void operator-=(const Vector2D<K>& v) { this->x -= v.x; this->y -= v.y; }

	template <typename K>
	inline void operator*=(const Vector2D<K>& v) { this->x *= v.x; this->y *= v.y; }

	template <typename K>
	inline void operator/=(const Vector2D<K>& v) { this->x /= v.x; this->y /= v.y; }

	template <typename K>
	inline void operator+=(const K& n) { this->x += n; this->y += n; }

	template <typename K>
	inline void operator-=(const K& n) { this->x -= n; this->y -= n; }

	template <typename K>
	inline void operator*=(const K& n) { this->x *= n; this->y *= n; }

	template <typename K>
	inline void operator/=(const K& n) { this->x /= n; this->y /= n; }

	template<typename K>
	[[nodiscard]] inline bool operator==(const Vector2D<K>& v) { return this->x == v.x && this->y == v.y; }

	template<typename K>
	[[nodiscard]] inline bool operator!=(const Vector2D<K>& v) { return this->x != v.x || this->y != v.y; }
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector2D<T>& v)
{
	os << '(' << v.x << ", " << v.y << ")";
	return os;
}

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator+(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x + b.x, a.y + b.y }; }

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator-(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x - b.x, a.y - b.y }; }

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator*(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x * b.x, a.y * b.y }; }

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator/(const Vector2D<T>& a, const Vector2D<K>& b) { return Vector2D<T>{ a.x / b.x, a.y / b.y }; }

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator+(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x + n, v.y + n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator-(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x - n, v.y - n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator*(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x * n, v.y * n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector2D<T> operator/(const Vector2D<T>& v, const K& n) { return Vector2D<T>{ v.x / n, v.y / n }; }

template <typename T>
struct Vector3D : Vector2D<T>
{
	T z;

	Vector3D() {}

	template <typename K>
	Vector3D(const K& n) : Vector2D<T>(n), z(static_cast<T>(n)) {}

	template <typename K, typename L, typename M>
	Vector3D(const K& x, const L& y, const M& z) : Vector2D<T>(x, y), z(static_cast<T>(z)) {}

	template <typename K>
	inline void operator+=(const Vector3D<K>& v) { this->x += v.x; this->y += v.y; this->z += v.z; }

	template <typename K>
	inline void operator-=(const Vector3D<K>& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; }

	template <typename K>
	inline void operator*=(const Vector3D<K>& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; }

	template <typename K>
	inline void operator/=(const Vector3D<K>& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; }

	template <typename K>
	inline void operator+=(const K& n) { this->x += n; this->y += n; this->z += n; }

	template <typename K>
	inline void operator-=(const K& n) { this->x -= n; this->y -= n; this->z -= n; }

	template <typename K>
	inline void operator*=(const K& n) { this->x *= n; this->y *= n; this->z *= n; }

	template <typename K>
	inline void operator/=(const K& n) { this->x /= n; this->y /= n; this->z /= n; }

	template<typename K>
	[[nodiscard]] inline bool operator==(const Vector3D<K>& v) { return this->x == v.x && this->y == v.y && this->z == v.z; }

	template<typename K>
	[[nodiscard]] inline bool operator!=(const Vector3D<K>& v) { return this->x != v.x || this->y != v.y || this->z != v.z; }
};

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator+(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x + b.x, a.y + b.y, a.z + b.z }; }

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator-(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x - b.x, a.y - b.y, a.z - b.z }; }

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator*(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x * b.x, a.y * b.y, a.z * b.z }; }

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator/(const Vector3D<T>& a, const Vector3D<K>& b) { return Vector3D<T>{ a.x / b.x, a.y / b.y, a.z / b.z }; }

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator+(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x + n, v.y + n, v.z + n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator-(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x - n, v.y - n, v.z - n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator*(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x * n, v.y * n, v.z * n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector3D<T> operator/(const Vector3D<T>& v, const K& n) { return Vector3D<T>{ v.x / n, v.y / n, v.z / n }; }

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector3D<T>& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ")";
	return os;
}

template <typename T>
struct Vector4D : Vector3D<T>
{
	T w;

	Vector4D() {}

	template <typename K>
	Vector4D(const K& n) : Vector3D<T>(n), w(static_cast<T>(w)) {}

	template <typename K, typename L, typename M, typename N>
	Vector4D(const K& x, const L& y, const M& z, const N& w) : Vector3D<T>(x, y, z), w(static_cast<T>(w)) {}

	template <typename K>
	inline void operator+=(const Vector4D<K>& v) { this->x += v.x; this->y += v.y; this->z += v.z; this->w += v.w; }

	template <typename K>
	inline void operator-=(const Vector4D<K>& v) { this->x -= v.x; this->y -= v.y; this->z -= v.z; this->w -= v.w; }

	template <typename K>
	inline void operator*=(const Vector4D<K>& v) { this->x *= v.x; this->y *= v.y; this->z *= v.z; this->w *= v.w; }

	template <typename K>
	inline void operator/=(const Vector4D<K>& v) { this->x /= v.x; this->y /= v.y; this->z /= v.z; this->w /= v.w; }

	template <typename K>
	inline void operator+=(const K& n) { this->x += n; this->y += n; this->z += n; this->w += n; }

	template <typename K>
	inline void operator-=(const K& n) { this->x -= n; this->y -= n; this->z -= n; this->w -= n; }

	template <typename K>
	inline void operator*=(const K& n) { this->x *= n; this->y *= n; this->z *= n; this->w *= n; }

	template <typename K>
	inline void operator/=(const K& n) { this->x /= n; this->y /= n; this->z /= n; this->w /= n; }

	template<typename K>
	[[nodiscard]] inline bool operator==(const Vector4D<K>& v) { return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;; }

	template<typename K>
	[[nodiscard]] inline bool operator!=(const Vector4D<K>& v) { return this->x != v.x || this->y != v.y || this->z != v.z || this->w != v.w;; }
};

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator+(const Vector4D<T>& a, const Vector4D<K>& b) { return Vector4D<T>{ a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator-(const Vector4D<T>& a, const Vector4D<K>& b) { return Vector4D<T>{ a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator*(const Vector4D<T>& a, const Vector4D<K>& b) { return Vector4D<T>{ a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; }

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator/(const Vector4D<T>& a, const Vector4D<K>& b) { return Vector4D<T>{ a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator+(const Vector4D<T>& v, const K& n) { return Vector4D<T>{ v.x + n, v.y + n, v.z + n, v.w + n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator-(const Vector4D<T>& v, const K& n) { return Vector4D<T>{ v.x - n, v.y - n, v.z - n, v.w - n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator*(const Vector4D<T>& v, const K& n) { return Vector4D<T>{ v.x * n, v.y * n, v.z * n, v.w * n }; }

template <typename T, typename K>
[[nodiscard]] inline Vector4D<T> operator/(const Vector4D<T>& v, const K& n) { return Vector4D<T>{ v.x / n, v.y / n, v.z / n, v.w / n }; }

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Vector4D<T>& v)
{
	os << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
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

typedef Vector4D<float>    Vec4f;
typedef Vector4D<int8_t>   Vec4i8;
typedef Vector4D<int16_t>  Vec4i16;
typedef Vector4D<int32_t>  Vec4i32;
typedef Vector4D<int64_t>  Vec4i64;
typedef Vector4D<uint8_t>  Vec4u8;
typedef Vector4D<uint16_t> Vec4u16;
typedef Vector4D<uint32_t> Vec4u32;
typedef Vector4D<uint64_t> Vec4u64;