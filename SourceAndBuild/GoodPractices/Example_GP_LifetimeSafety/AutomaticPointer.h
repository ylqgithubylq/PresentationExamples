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
		Resource()
		{
		}
		~Resource()
		{
		}
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

		void SetBig(unique_ptr<Big> b)
		{
			big = move(b);
		}

		void SetResource(shared_ptr<Resource> r)
		{
			resource = move(r);
		}
	};

	void UseBig(Big const& big)
	{ // dummy example user
	}

	void UseResource(Resource const& resource)
	{ // dummy example user
	}

	void Usage()
	{
		// probably get from resource manager
		shared_ptr<Resource> resource = make_shared<Resource>();

		Object object(resource);
		UseBig(object.GetBig());
		if (object.GetResource())
		{
			UseResource(*object.GetResource());
		}
	}

	struct Manager
	{
		Resource* CreateResource()
		{
			return new Resource;
		}
		void ReleaseResource(Resource* resource)
		{
			resource = resource;
			delete resource;
		}
	};

	void Decouple()
	{
		Manager manager;
		auto resource = shared_ptr<Resource>(manager.CreateResource(), [&manager] (Resource* resource)
		{
			manager.ReleaseResource(resource);
		});
		Object object(resource);
		UseResource(*object.GetResource());

		struct ResourceDeleter 
		{
			Manager& manager;
			ResourceDeleter(Manager& manager)
				: manager(manager)
			{
			}
			void operator()(Resource* resource)
			{
				manager.ReleaseResource(resource);
			}
		};

		auto up = unique_ptr<Resource, ResourceDeleter>(manager.CreateResource(), ResourceDeleter(manager));
		UseResource(*up);
	}


	void Weak()
	{	
		Object object(nullptr);
		object.SetResource(make_shared<Resource>());

		auto ob = weak_ptr<Resource>(object.resource);
		if (!ob.expired())
		{
			auto resource = ob.lock();
			UseResource(*resource);
		}

		object.SetResource(make_shared<Resource>()); // set a new resource
		
		if (!ob.expired())
		{ // will not get here
			auto resource = ob.lock();
			UseResource(*resource);
		}
	}

	void Run()
	{
		Usage();
		Decouple();
		Weak();
	}
}