#pragma once
#include "CommonTools.h"
#include <vector>
#include <type_traits>
#include <unordered_map>
#include <string>

namespace TypeDeduction
{
	using namespace std;

	void Auto()
	{
		vector<int> is;
		for (vector<int>::iterator i = is.begin(); i != is.end(); i++)
		{
			int temp = *i;
		}
		for (auto i = is.begin(); i != is.end(); i++)
		{
			int temp = *i;
		}

		unordered_map<string, vector<string>> cs;
		for (unordered_map<string, vector<string>>::iterator i = cs.begin(); i != cs.end(); i++)
		{
			vector<string>& s = i->second;
		}
		for (auto i = cs.begin(); i != cs.end(); i++)
		{
			vector<string>& s = i->second;
		}
	}

	template <typename T, typename Y>
	auto Add1(T const& left, Y const& right) -> decltype(left + right)
	{
		return left + right;
	}
	template <typename T, typename Y>
	auto Add2(T const& left, Y const& right)
	{
		return left + right;
	}

	void Decltype()
	{
		auto i = 1;
		decltype(i) di = i;
		decltype((i)) dri = i;

		auto result1 = Add1(1, 1.f);
		auto result2 = Add2(1, 1.f);

		static_assert(is_same<decltype(result1), float>::value, "");
		static_assert(is_same<decltype(result2), float>::value, "");
	}

	void Run()
	{
		Auto();
		Decltype();
	}
}
