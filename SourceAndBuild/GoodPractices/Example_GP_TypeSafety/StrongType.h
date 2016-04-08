#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <array>
#include <cmath>

namespace StrongType
{
	using namespace std;

	struct Vector3
	{
		constexpr static size_t Dim = 3;
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};
			array<float, Dim> v;
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
		float operator[](size_t index) const
		{
			return v[index];
		}

		Vector3& operator+=(Vector3 const& other)
		{
			for (size_t i = 0; i < Dim; i++)
			{
				v[i] += other.v[i];
			}
			return *this;
		}

		Vector3& operator-=(Vector3 const& other)
		{
			for (size_t i = 0; i < Dim; i++)
			{
				v[i] -= other.v[i];
			}
			return *this;
		}

		Vector3& operator*=(float f)
		{
			for (size_t i = 0; i < Dim; i++)
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

	float Length(Vector3 const& v)
	{
		float lengthSquared = 0;
		for (size_t i = 0; i < Vector3::Dim; i++)
		{
			lengthSquared += v[i] * v[i];
		}
		return lengthSquared;
		return sqrt(lengthSquared);
	}


	Vector3 Normalize(Vector3 const& v)
	{
		float length = Length(v);
		Vector3 result = v * (1 / length);
		return result;
	}

	struct Difference3;

	struct Position3
	{
		Vector3 v;

		Position3() = default;
		explicit Position3(Vector3 const& v)
			: v(v)
		{
		}
		Position3(float x, float y, float z)
			: v(x, y, z)
		{
		}
	};

	struct Difference3
	{
		Vector3 v;
		Difference3() = default;
		explicit Difference3(Vector3 const& v)
			: v(v)
		{
		}
		Difference3(float x, float y, float z)
			: v(x, y, z)
		{
		}
	};

	struct Direction3
	{
		Vector3 v;
		Direction3() = default;
		explicit Direction3(Vector3 const& v)
			: v(v)
		{
			assert(abs(Length(this->v) - 1) < 1e-7);
		}
		Direction3(float x, float y, float z)
			: v(x, y, z)
		{
			assert(abs(Length(this->v) - 1) < 1e-7);
		}
		Direction3(Difference3 const& d)
			: v(Normalize(d.v))
		{
		}
	};

	Difference3 operator-(Position3 const& left, Position3 const& right)
	{
		return Difference3(left.v - right.v);
	}
	Position3 operator+(Difference3 const& left, Position3 const& right)
	{
		return Position3(left.v + right.v);
	}
	Position3 operator+(Position3 const& left, Difference3 const& right)
	{
		return Position3(left.v + right.v);
	}
	Difference3 operator+(Difference3 const& left, Difference3 const& right)
	{
		return Difference3(left.v - right.v);
	}
	Difference3 operator*(Difference3 const& left, float right)
	{
		return Difference3(left.v * right);
	}
	Difference3 operator*(float left, Difference3 const& right)
	{
		return Difference3(left * right.v);
	}

	Difference3 operator*(Direction3 const& left, float right)
	{
		return Difference3(left.v * right);
	}
	Difference3 operator*(float left, Direction3 const& right)
	{
		return Difference3(left * right.v);
	}

	Direction3 Normalize(Difference3 const& d)
	{
		return Direction3(Normalize(d.v));
	}

	struct Degree;

	struct Radian
	{
		float v;
		Radian() = default;
		explicit Radian(float f)
			: v(f)
		{
		}
		Radian(Degree d);
	};


	struct Degree
	{
		float v;
		Degree() = default;
		explicit Degree(float f)
			: v(f)
		{
		}
		Degree(Radian d);
	};

	Radian::Radian(Degree d)
		: v(d.v / 180 * 3.14159265358979f)
	{
	}
	Degree::Degree(Radian d)
		: v(d.v * 180 / 3.14159265358979f)
	{
	}



	void Use()
	{
		struct UnitQuaternion
		{
			array<float, 4> v;
		};
		struct ExampleMathFunctions
		{
			static UnitQuaternion RotationFromTo(Direction3 const& from, Direction3 const& to)
			{
				// implementation omitted
				return UnitQuaternion();
			}

			static UnitQuaternion RotationAround(Direction3 const& axis, Radian angle)
			{
				// implementation omitted
				return UnitQuaternion();
			}

			static Difference3 Rotate(UnitQuaternion const& rotation, Difference3 const& d)
			{
				// implementation omitted
				return Difference3();
			}

			static Direction3 Rotate(UnitQuaternion const& rotation, Direction3 const& d)
			{
				// implementation omitted
				return Direction3();
			} 
		};

		Position3 a(1, 2, 3);
		Position3 b(2, 3, 4);
		Position3 c(3, 4, 5);
		Difference3 d0 = b - a;
		Difference3 d1 = c - a;
		
		// no need to worry about whether d0 and d1 are normalized, and they will be normalized when needed.
		UnitQuaternion q0 = ExampleMathFunctions::RotationFromTo(d0, d1);
		UnitQuaternion q1 = ExampleMathFunctions::RotationAround(d0, Degree(90));

		Difference3 rotatedD0 = ExampleMathFunctions::Rotate(q0, d0);
	}


	void Run()
	{
		Use();
	}
}