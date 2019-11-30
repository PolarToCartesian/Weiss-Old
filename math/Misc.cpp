#include "Misc.h"

Vec3f Misc::getTriangleSurfaceNormal(const Vec3f& a, const Vec3f& b, const Vec3f& c)
{
	const Vec3f U = b - a;
	const Vec3f V = c - a;

	return Vec3f {
		U[1] * V[2] - U[2] * V[1],
		U[2] * V[0] - U[0] * V[2],
		U[0] * V[1] - U[1] * V[0]
	};
}