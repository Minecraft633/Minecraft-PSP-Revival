#ifndef AURORA_MATHLIB_H
#define AURORA_MATHLIB_H

#include <cmath>
#include <cstdlib>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{
		class Math
		{
		public:
			static const float PI;
			static const float HALF_PI;
			static const float QUARTER_PI;
			static const float TWO_PI;
			static const float EPSILON;

			template <typename T>
			static T bilerp(const T &a, const T &b, const T &c, const T &d, float u, float v)
			{
				return a * ((1.0f - u) * (1.0f - v))
					+ b * (u * (1.0f - v))
					+ c * (v * (1.0f - u))
					+ d * (u * v);
			}

			static void cartesianToSpherical(float x, float y, float z,
				float &rho, float &phi, float &theta)
			{
				rho = sqrtf((x * x) + (y * y) + (z * z));
				phi = asinf(y / rho);
				theta = atan2f(z, x);
			}

			static bool closeEnough(float f1, float f2)
			{
				return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
			}

			static float degreesToRadians(float degrees)
			{
				return (degrees * 3.1415926f) / 180.0f;
			}

			static bool isPower2(int x)
			{
				return ((x > 0) && ((x & (x - 1)) == 0));
			}

			template <typename T>
			static T lerp(const T &a, const T &b, float t)
			{
				return a + (b - a) * t;
			}

			static int nextMultipleOf(int multiple, int value)
			{
				return multiple * ((value + (multiple - 1)) / multiple);
			}

			static int nextPower2(int x);

			static float radiansToDegrees(float radians)
			{
				return (radians * 180.0f) / 3.1415926f;
			}

			static float random(float min, float max)
			{
				return min + (max - min)
					* (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
			}

			static float smoothstep(float a, float b, float x)
			{
				if (x < a)
				{
					return 0.0f;
				}
				else if (x >= b)
				{
					return 1.0f;
				}
				else
				{
					x = (x - a) / (b - a);
					return x * x * (3.0f - 2.0f * x);
				}
			}

			static void sphericalToCartesian(float rho, float phi, float theta,
				float &x, float &y, float &z)
			{
				x = rho * cosf(phi) * cosf(theta);
				y = rho * sinf(phi);
				z = rho * cosf(phi) * sinf(theta);
			}
		};
	}
}
#endif