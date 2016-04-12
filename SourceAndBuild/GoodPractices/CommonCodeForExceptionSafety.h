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
		uint64_t Calculate(uint64_t index) noexcept
		{
			return Fibonacci(index);
		}

		virtual uint64_t Fibonacci(uint64_t index) = 0;

		virtual uint64_t DoFibonacci(uint64_t index) = 0;

		virtual pair<uint64_t, bool> DoFibonacciByError(uint64_t index) = 0;
	};

	struct NormalFibonacciCalculator : IFibonacciCalculator
	{
		virtual uint64_t Fibonacci(uint64_t index) noexcept override
		{
			return DoFibonacci(index);
		}

		virtual uint64_t DoFibonacci(uint64_t index) noexcept override
		{
			if (index == 0)
			{
				return 1;
			}
			else if (index == 1)
			{
				return 1;
			}
			else
			{
				return DoFibonacci(index - 1) + DoFibonacci(index - 2);
			}
		}

		virtual pair<uint64_t, bool> DoFibonacciByError(uint64_t index) override
		{
			return{ 0, true };
		}
	};

	struct ExceptionFibonacciCalculator : IFibonacciCalculator
	{
		struct Reach1
		{
		};


		virtual uint64_t Fibonacci(uint64_t index) noexcept override
		{
			uint64_t i = 0;
			try
			{
				i = DoFibonacci(index);
			}
			catch (const Reach1&)
			{
				i = 1;
			}
			return i;
		}

		virtual uint64_t DoFibonacci(uint64_t index) override
		{
			if (index <= 1)
			{
				throw Reach1();
			}
			else
			{
				uint64_t i0 = 0;
				uint64_t i1 = 0;
				try
				{
					i0 = DoFibonacci(index - 1);
				}
				catch (const Reach1&)
				{
					i0 = 1;
				}
				try
				{
					i1 = DoFibonacci(index - 2);
				}
				catch (const Reach1&)
				{
					i1 = 1;
				}
				return  i0 + i1;
			}
		}

		virtual pair<uint64_t, bool> DoFibonacciByError(uint64_t index) override
		{
			return{ 0, true };
		}
	};

	struct ErrorFibonacciCalculator : IFibonacciCalculator
	{
		virtual uint64_t Fibonacci(uint64_t index) noexcept override
		{
			uint64_t i = 0;
			bool reach1 = false;
			
			tie(i, reach1) = DoFibonacciByError(index);
			if (reach1)
			{
				i = 1;
			}

			return i;
		}

		virtual uint64_t DoFibonacci(uint64_t index) override
		{
			return 0;
		}

		virtual pair<uint64_t, bool> DoFibonacciByError(uint64_t index) override
		{
			if (index <= 1)
			{
				return{ 0, true };
			}
			else
			{
				pair<uint64_t, bool> i0 = DoFibonacciByError(index - 1);
				if (i0.second)
				{
					i0.first = 1;
				}
				pair<uint64_t, bool> i1 = DoFibonacciByError(index - 2);
				if (i1.second)
				{
					i1.first = 1;
				}
				return{ i0.first + i1.first, false };
			}
		}
	};

}
