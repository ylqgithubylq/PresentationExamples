#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <array>

namespace AutomaticPointer
{
	using namespace std;

	void Basic()
	{
		unique_ptr<int> upi1 = make_unique<int>(42);
#ifdef TurnOnCompileErrorCode
		unique_ptr<int> upi2 = upi1; // compile error
#endif
		shared_ptr<int> spi1 = make_shared<int>(0);
		shared_ptr<int> spi2 = spi1;

		spi2 = move(upi1); // construct shared_ptr from unique_ptr and upi1 will be nullptr

		int* pi1 = spi2.get();
		int& ri1 = *spi2;
		ri1 = *spi1;
	}

	struct Big
	{
		array<int, 1024 * 1024> dummy;
	};
	struct Resource
	{
		array<int, 1024 * 1024> dummy;
	};

	struct Object
	{
		unique_ptr<Big> big;
		shared_ptr<Resource> resource;

		Object(shared_ptr<Resource> resource)
			: big(make_unique<Big>()), resource(move(resource))
		{ // big will not be nullptr
		}

		Big& GetBig() // not null
		{
			return *big;
		}

		Resource* GetResource() // may null
		{
			return resource.get();
		}
	};

	void UseBig(Big const& big)
	{
	}

	void UseResource(Resource const& resource)
	{
	}

	void Usage()
	{
		// probably get from resource manager
		shared_ptr<Resource> r = make_shared<Resource>();

		Object o(r);
		UseBig(o.GetBig());
		if (o.GetResource())
		{
			UseResource(*o.GetResource());
		}
	}

	void Run()
	{
		Usage();
	}
}