#pragma once
#include "CommonTools.h"
#include "../CommonCodeForExceptionSafety.h"

#include <utility>
#include <functional>
#include <vector>
#include <memory>
#include <cassert>
#include <array>
#include <iostream>

namespace PerfomanceComparison
{
	using namespace std;

	void Run()
	{
		auto errorIndex = make_unique<int64_t>(-1);
		auto index = make_unique<int64_t>(25);
		auto r0 = make_unique<int64_t>(0);
		auto r1 = make_unique<int64_t>(0);
		auto r2 = make_unique<int64_t>(0);

		auto normalTime = MeasureExecutionTime([&]
		{
			ExceptionSafety::NormalFibonacciCalculator cc;
			*r0 = cc.Calculate(*errorIndex);
			function<int64_t(int64_t)> caller = [cc] (int64_t index) mutable
			{
				return cc.Calculate(index);
			};
			*r0 += caller(*index);
		});
		auto errorTime = MeasureExecutionTime([&]
		{
			ExceptionSafety::FibonacciCalculatorWithError cc;
			*r1 = cc.Calculate(*errorIndex);
			function<int64_t(int64_t)> caller = [cc] (int64_t index) mutable
			{
				return cc.Calculate(index);
			};
			*r1 += caller(*index);
		});
		auto exceptionTime = MeasureExecutionTime([&]
		{
			ExceptionSafety::FibonacciCalculatorWithException cc;
			*r2 = cc.Calculate(*errorIndex);
			function<int64_t(int64_t)> caller = [cc] (int64_t index) mutable
			{
				return cc.Calculate(index);
			};
			*r2 += caller(*index);
		});


		cout << "exception disabled." << endl;
		cout << "normal time : " << normalTime.count() << ", error time : " << errorTime.count() << ", exception time : " << exceptionTime.count() << endl;
	}
}