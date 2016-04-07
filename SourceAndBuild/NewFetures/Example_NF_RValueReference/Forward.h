#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

namespace Forward
{
	using namespace std;

	// impossible if used as Forward<T>(parameter)
// 	template <typename T>
// 	T&& Forward(T&& v)
// 	{
// 		using Type = T&&;
// 		return (static_cast<T&&>(v));
// 	}
	template <typename T>
	T&& Forward(T& v)
	{
		using Type = T&&;
		return (static_cast<T&&>(v));
	}

	
	void Inner(string const& s)
	{
		cout << "string const& s: " << s << endl;
	}

	void Inner(string&& s)
	{
		cout << "string&& s: " << s << endl;
	}


	template <typename T>
	void OuterExpanded(T&& v) // what it look like when writing directly
	{
		Inner(static_cast<T&&>(v));
	}

	template <typename T>
	void Outer(T&& v)
	{
		Inner(Forward<T>(v));
	}




	void Run()
	{
		string lv("lv");
		OuterExpanded(lv);
		OuterExpanded(string("rv"));
		Outer(lv);
		Outer(string("rv"));
	}
}