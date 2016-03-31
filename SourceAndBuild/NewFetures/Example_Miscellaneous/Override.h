#pragma once
#include "CommonTools.h"

namespace Override
{
	struct Base
	{
		virtual ~Base() = default;
		virtual void F() = 0;
	};

	struct Derived : Base
	{
#ifdef TurnOnCompileErrorCode
		virtual void FF() override
		{
		}
		virtual int F() override
		{
		}
#else
		virtual void F() override
		{
		}
#endif
	};
}