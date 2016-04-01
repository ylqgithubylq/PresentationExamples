#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <string>
#include <cassert>
#include <type_traits>

namespace RValueTemplate
{
	using namespace std;

	template <typename T>
	void F(T&& o)
	{
		const bool lvr = is_lvalue_reference<T>::value;
		const bool rvr = is_rvalue_reference<T>::value;
		cout << "lv: " << lvr << ", rv: " << rvr << endl;
	}

	void Run()
	{
		int i = 0;
		int& ri = i;
		F(ri); // F<int&>
		F(1); // F<int>
	}
}