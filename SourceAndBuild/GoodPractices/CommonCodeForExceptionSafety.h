#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <array>
#include <tuple>

namespace ExceptionSafety
{
	using namespace std;

	struct IFibonacciCalculator
	{
		virtual ~IFibonacciCalculator() = default;
		int64_t Calculate(int64_t index) noexcept
		{
			return Fibonacci(index);
		}

		virtual int64_t Fibonacci(int64_t index) = 0;

		virtual int64_t DoFibonacci(int64_t index) = 0;

		virtual pair<int64_t, bool> DoFibonacciByError(int64_t index) = 0;
	};

	struct NormalFibonacciCalculator : IFibonacciCalculator
	{
		virtual int64_t Fibonacci(int64_t index) noexcept override
		{
			return DoFibonacci(index);
		}

		virtual int64_t DoFibonacci(int64_t index) noexcept override
		{
			if (index < 0)
			{
				return 0;
			}
			else if (index <= 1)
			{
				return 1;
			}
			else
			{
				return DoFibonacci(index - 1) + DoFibonacci(index - 2);
			}
		}

		virtual pair<int64_t, bool> DoFibonacciByError(int64_t index) override
		{
			return{ 0, true };
		}
	};

	struct FibonacciCalculatorWithException : IFibonacciCalculator
	{
		struct SmallerThan0
		{
		};


		virtual int64_t Fibonacci(int64_t index) noexcept override
		{
			int64_t i = 0;
			try
			{
				i = DoFibonacci(index);
			}
			catch (const SmallerThan0&)
			{
				i = 0;
			}
			return i;
		}

		virtual int64_t DoFibonacci(int64_t index) override
		{
			if (index < 0)
			{
				throw SmallerThan0();
			}
			else if (index <= 1)
			{
				return 1;
			}
			else
			{
				int64_t i0 = 0;
				int64_t i1 = 0;
				try
				{
					i0 = DoFibonacci(index - 1);
				}
				catch (const SmallerThan0&)
				{
					i0 = 0;
				}
				try
				{
					i1 = DoFibonacci(index - 2);
				}
				catch (const SmallerThan0&)
				{
					i1 = 0;
				}
				return  i0 + i1;
			}
		}

		virtual pair<int64_t, bool> DoFibonacciByError(int64_t index) override
		{
			return{ 0, true };
		}
	};

	struct FibonacciCalculatorWithError : IFibonacciCalculator
	{
		virtual int64_t Fibonacci(int64_t index) noexcept override
		{
			int64_t i = 0;
			bool error = false;
			
			tie(i, error) = DoFibonacciByError(index);

			return i;
		}

		virtual int64_t DoFibonacci(int64_t index) override
		{
			return 0;
		}

		virtual pair<int64_t, bool> DoFibonacciByError(int64_t index) override
		{
			if (index < 0)
			{
				return{ 0, false };
			}
			else if (index <= 1)
			{
				return{ 1, true };
			}
			else
			{
				pair<int64_t, bool> i0 = DoFibonacciByError(index - 1);
				if (!i0.second)
				{
					i0.first = 0;
				}
				pair<int64_t, bool> i1 = DoFibonacciByError(index - 2);
				if (!i1.second)
				{
					i1.first = 0;
				}
				return{ i0.first + i1.first, true };
			}
		}
	};

}
