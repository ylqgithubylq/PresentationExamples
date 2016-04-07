#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <array>

namespace StrongType
{
	using namespace std;

	struct Vector3
	{
		union
		{
			array<float, 3> v;
			struct
			{
				float x;
				float y;
				float z;
			};
		};

		Vector3() = default;

		Vector3(float x, float y, float z) 
			: x(x), y(y), z(z)
		{
		}

		float& operator[](size_t index)
		{
			return v[index];
		}
		float& operator[](size_t index)
		{
			return v[index];
		}

		Vector3& operator+=(Vector3 const& other)
		{
			for (size_t i = 0; i < 3; i++)
			{
				v[i] += other.v[i];
			}
			return *this;
		}

		Vector3& operator-=(Vector3 const& other)
		{
			for (size_t i = 0; i < 3; i++)
			{
				v[i] -= other.v[i];
			}
			return *this;
		}

		Vector3& operator*=(float f)
		{
			for (size_t i = 0; i < 3; i++)
			{
				v[i] *= f;
			}
			return *this;
		}

		friend Vector3 operator+(Vector3 const& left, Vector3 const& right)
		{
			Vector3 result = left;
			result += right;
			return result;
		}

		friend Vector3 operator-(Vector3 const& left, Vector3 const& right)
		{
			Vector3 result = left;
			result -= right;
			return result;
		}

		friend Vector3 operator*(Vector3 const& left, float right)
		{
			Vector3 result = left;
			result *= right;
			return result;
		}

		friend Vector3 operator*(float left, Vector3 const& right)
		{
			Vector3 result = right;
			result *= left;
			return result;
		}
	};

	void Run()
	{
		Vector3 v1(1.f, 2.f, 3.f);
		Vector3 v2 = v1 + v1;
		Vector3 v3 = v2 * 3;
	}
}