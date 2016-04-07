#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <tuple>
#include <functional>
#include <algorithm>
#include <cassert>
#include <array>
#include <memory>
#include <iostream>

namespace Function
{
	using namespace std;

	void Basic()
	{
		function<int(double)> f;
		f = [] (double d)
		{
			return static_cast<int>(d);
		};

		int result = f(3.14);
		assert(result == 3);
	}

	void Compatiable()
	{
		struct Base
		{
			virtual ~Base() = default;
		};
		struct Derived : Base
		{
		};

		// return: Covariance
		// parameters: Contravariance
		function<Base*(float, Derived*)> f = [] (double, Base* b) -> Derived*
		{
			return dynamic_cast<Derived*>(b);
		};

		// generic lambda
		f = [] (auto&&... v)
		{
			return static_cast<Derived*>(nullptr);
		};

		Derived d;
		f(3.14f, &d);
	}

	void Performance()
	{
		struct Dummy
		{
			array<int, 16> dummy;
		};
		Dummy dummy;

		auto lambda = [dummy] (double d)
		{
			return make_pair(d * d, d + d);
		};

		function<pair<double, double>(double)> func = lambda; // create func using a copy of lambda

		unique_ptr<int> loopCount = make_unique<int>(10000);

		unique_ptr<double> sumLambda0 = make_unique<double>(0);
		unique_ptr<double> sumLambda1 = make_unique<double>(0);
		auto lambdaTime = MeasureExecutionTime([&lambda, &loopCount, &sumLambda0, &sumLambda1] ()
		{
			int n = *loopCount;
			for (int i = 0; i < n; i++)
			{
				auto result = lambda(i);
				*sumLambda0 += result.first;
				*sumLambda1 += result.second;
			}
		});

		unique_ptr<double> sumFunction0 = make_unique<double>(0);
		unique_ptr<double> sumFunction1 = make_unique<double>(0);
		auto functionTime = MeasureExecutionTime([&func, &loopCount, &sumFunction0, &sumFunction1] ()
		{
			int n = *loopCount;
			for (int i = 0; i < n; i++)
			{
				auto result = func(i);
				*sumFunction0 += result.first;
				*sumFunction1 += result.second;
			}
		});

		cout << "lambda time: " << lambdaTime.count() << ", function time: " << functionTime.count() << endl;
	}

	void Run()
	{
		Basic();
		Compatiable();
		Performance();
	}
}