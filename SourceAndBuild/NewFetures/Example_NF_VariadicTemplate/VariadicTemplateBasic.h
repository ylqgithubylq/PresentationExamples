#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <tuple>
#include <cassert>

namespace VariadicTemplateBasic
{
	using namespace std;
	
	template <typename Type>
	void AddImpl(vector<Type>& container)
	{
	}
	template <typename Type, typename... Types>
	void AddImpl(vector<Type>& container, Type&& i, Types&&... is)
	{
		container.push_back(forward<Type>(i));
		AddImpl(container, forward<Types>(is)...);
	}

	template <typename Type, typename... Types>
	void Add(vector<Type>& container, Types&&... is)
	{
		AddImpl(container, forward<Types>(is)...);
	}

	void Run()
	{
		tuple<double, int, string> fis(3.14, 42, string("s"));
		assert(get<0>(fis) == 3.14);

		vector<int> is;
		Add(is);
		Add(is, 1);
		Add(is, 2, 3, 4, 5);
		assert(is == vector<int>({ 1, 2, 3, 4, 5 }));
	}
}