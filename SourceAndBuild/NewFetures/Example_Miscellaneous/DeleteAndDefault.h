#pragma once
#include "CommonTools.h"
#include <utility>

namespace DeleteAndDefault
{
	using namespace std;
	struct Object
	{
// 		Object() = default;
// 		~Object() = default;
// 
// 		Object(Object const& other) = default;
// 		Object& operator=(Object const& other) = delete;
// 
// 		Object(Object&& other) = default;
// 		Object& operator=(Object&& other) = delete;
// 		
	};

	void Run()
	{
		static_assert(is_trivial<Object>::value, "");

		Object object; // call Object()

		Object otherObject1 = move(object); // call Object(Object&& other)

#ifdef TurnOnCompileErrorCode
		Object otherObject2;
		otherObject2 = otherObject1; // compile error, deleted function
#endif
	}

}

