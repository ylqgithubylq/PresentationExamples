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
		auto index = make_unique<uint64_t>(20);
		auto r0 = make_unique<uint64_t>(0);
		auto r1 = make_unique<uint64_t>(0);
		auto r2 = make_unique<uint64_t>(0);

		auto normalTime = MeasureExecutionTime([&]
		{
			ExceptionSafety::NormalFibonacciCalculator cc;
			function<uint64_t(uint64_t)> caller = [cc] (uint64_t index) mutable
			{
				return cc.Calculate(index);
			};
			*r0 = caller(*index);
		});
		auto errorTime = MeasureExecutionTime([&]
		{
			ExceptionSafety::ErrorFibonacciCalculator cc;
			function<uint64_t(uint64_t)> caller = [cc] (uint64_t index) mutable
			{
				return cc.Calculate(index);
			};
			*r1 = caller(*index);
		});
		auto exceptionTime = MeasureExecutionTime([&]
		{
			ExceptionSafety::ExceptionFibonacciCalculator cc;
			function<uint64_t(uint64_t)> caller = [cc] (uint64_t index) mutable
			{
				return cc.Calculate(index);
			};
			*r2 = caller(*index);
		});

		cout << "exception enabled." << endl;
		cout << "normal time: " << normalTime.count() << ", error time: " << errorTime.count() << ", exception time: " << exceptionTime.count() << endl;
	}
}