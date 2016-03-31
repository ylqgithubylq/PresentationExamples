#pragma once
#include "CommonTools.h"
#include <vector>
#include <memory>
#include <chrono>
#include <iostream>

namespace RangeBasedForLoop
{
	using namespace std;

	void Use()
	{
		vector<int> is;
		for (int& i : is)
		{
			i = 42;
		}
	}

	struct Range
	{
		struct Iterator
		{
			int current;

			Iterator(int value)
				: current(value)
			{
			}

			Iterator& operator++() // ++i
			{
				current += 1;
				return *this;
			}

			int operator*()
			{
				return current;
			}

			friend bool operator!=(Iterator const& left, Iterator const& right)
			{
				return left.current != right.current;
			}
		};

		int startValue = 0;
		int endValue = 0;

		Range() = default;
		Range(int s, int e)
			: startValue(s), endValue(e)
		{
		}

		Iterator begin() const
		{
			return Iterator(startValue);
		}
		Iterator end() const
		{
			return Iterator(endValue);
		}
	};

	Range::Iterator begin(Range const& range)
	{
		return Range::Iterator(range.startValue);
	}
	Range::Iterator end(Range const& range)
	{
		return Range::Iterator(range.endValue);
	}

	void UserDefined()
	{
		auto dummy = Now();
		
		auto rangeEndHolder = make_unique<int>(100000);
		int rangeEnd = *rangeEndHolder;

		int sum0 = 0;
		auto rangeTime = ExecutionTime([&sum0, rangeEnd]
		{
			for (int i : Range(0, rangeEnd))
			{
				sum0 += i;
			}
		});
		auto toPreventSumOptimizedAway0 = make_unique<int>(sum0);

		int sum1 = 0;
		auto forTime = ExecutionTime([&sum1, rangeEnd]
		{
			for (int i = 0; i != rangeEnd; i++)
			{
				sum1 += i;
			}
		});
		auto toPreventSumOptimizedAway1 = make_unique<int>(sum1);


		(sum0 == sum1);
		cout << "range time: " << rangeTime.count() << ", for time: " << forTime.count() << "." << endl;
	}

	void Run()
	{
		Use();
		UserDefined();
	}
}
