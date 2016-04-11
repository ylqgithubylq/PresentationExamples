#pragma once
#include "CommonTools.h"
#include <utility>

namespace DeleteAndDefault
{
	using namespace std;
	struct Object
	{
		Object() = default;
		~Object() = default;

		Object(Object const& other) = delete;
		Object& operator=(Object const& other) = delete;

		Object(Object&& other) = default;
		Object& operator=(Object&& other) = delete;

		void Dummy()
		{
		}
	};

	Object CreateObject()
	{
		return Object();
	}

	void Run()
	{
		// not trivial, because Object has no move operator=
		static_assert(!is_trivial<Object>::value, "");

		Object object; // call Object()
		object.Dummy(); // to avoid the warning

		Object otherObject1 = CreateObject(); // call Object(Object&& other)

#ifdef TurnOnCompileErrorCode
		Object otherObject2;
		otherObject2 = otherObject1; // compile error, deleted function
#endif
	}

}

