#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>
namespace LambdaBasic
{
	using namespace std;

	void Old()
	{
		vector<int> is = { 35, 91, 34, 17, 68, 84, 21 };
		
		int mod = 10;

		struct Sorter
		{
			int mod;
			Sorter(int mod)
				: mod(mod)
			{
			}
			auto operator()(int left, int right) const -> bool
			{
				return left % mod < right % mod;
			}
		};
		stable_sort(is.begin(), is.end(), Sorter(mod));

		assert(is == vector<int>({ 91, 21, 34, 84, 35, 17, 68 }));
	}

	void New()
	{
		vector<int> is = { 35, 91, 34, 17, 68, 84, 21 };

		int mod = 10;

		stable_sort(is.begin(), is.end(), [mod] (int left, int right)
		{
			return left % mod < right % mod;
		});

		assert(is == vector<int>({ 91, 21, 34, 84, 35, 17, 68 }));
	}


	void Run()
	{
		Old();
		New();
	}
}