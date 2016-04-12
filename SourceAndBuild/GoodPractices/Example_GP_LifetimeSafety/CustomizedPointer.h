#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <array>
#include <iostream>

namespace CustomizedPointer
{
	using namespace std;

	
	template <typename T1, typename T2, bool T1Empty, bool T2Empty>
	struct CompressedPairImpl
	{
		T1 e1;
		T2 e2;

		template <typename P1, typename P2>
		CompressedPairImpl(P1&& p1, P2&& p2)
			: e1(forward<P1>(p1)), e2(forward<P2>(p2))
		{
		}

		T1& E1()
		{
			return e1;
		}
		T1 const& E1() const
		{
			return e1;
		}

		T2& E2()
		{
			return e2;
		}
		T2 const& E2() const
		{
			return e2;
		}
	};

	template <typename T1, typename T2>
	struct CompressedPairImpl<T1, T2, true, false>
		: T1
	{
		T2 e2;

		template <typename P1, typename P2>
		CompressedPairImpl(P1&& p1, P2&& p2)
			: T1(forward<P1>(p1)), e2(forward<P2>(p2))
		{
		}

		T1& E1()
		{
			return *this;
		}
		T1 const& E1() const
		{
			return *this;
		}

		T2& E2()
		{
			return e2;
		}
		T2 const& E2() const
		{
			return e2;
		}
	};

	template <typename T1, typename T2>
	struct CompressedPairImpl<T1, T2, false, true>
		: T2
	{
		T1 e1;

		template <typename P1, typename P2>
		CompressedPairImpl(P1&& p1, P2&& p2)
			: T2(forward<P2>(p2)), e1(forward<P1>(p1))
		{
		}

		T1& E1()
		{
			return e1;
		}
		T1 const& E1() const
		{
			return e1;
		}

		T2& E2()
		{
			return *this;
		}
		T2 const& E2() const
		{
			return this;
		}
	};

	template <typename T1, typename T2>
	struct CompressedPairImpl<T1, T2, true, true>
		: CompressedPairImpl<T1, T2, true, false>
	{
		using CompressedPairImpl<T1, T2, true, false>::CompressedPairImpl;
	};


	template <typename T1, typename T2>
	struct CompressedPair
		: CompressedPairImpl<T1, T2, is_empty<T1>::value, is_empty<T2>::value>
	{
		CompressedPair()
			: CompressedPairImpl<T1, T2, is_empty<T1>::value, is_empty<T2>::value>(T1(), T2())
		{
		}

		template <typename P1, typename P2>
		CompressedPair(P1&& p1, P2&& p2)
			: CompressedPairImpl<T1, T2, is_empty<T1>::value, is_empty<T2>::value>(forward<P1>(p1), forward<P2>(p2))
		{
		}
	};


	template <typename T>
	struct DefaultDeleter
	{
		void operator()(T* p)
		{
			delete p;
		}
	};

	// for illustration purpose only
	// very bad implemention
	template <typename T, typename Deleter = DefaultDeleter<T>>
	struct NoWeakPtr
	{
		NoWeakPtr()
		{
		}
		NoWeakPtr(nullptr_t)
		{
		}
		explicit NoWeakPtr(T* p)
			: p(p)
		{
			static_assert(is_same<Deleter, DefaultDeleter<T>>::value, "only NoWeakPtr using default deleter can construct without deleter");
		}

		NoWeakPtr(T* p, Deleter const& deleter)
			: p(p), rc(nullptr, deleter)
		{
		}
		NoWeakPtr(T* p, Deleter& deleter)
			: p(p), rc(nullptr, deleter)
		{
		}
		NoWeakPtr(T* p, Deleter&& deleter)
			: p(p), rc(nullptr, move(deleter))
		{
		}

		NoWeakPtr(NoWeakPtr<T, Deleter> const& other)
		{
			Set(other);
		}
		NoWeakPtr(NoWeakPtr<T, Deleter>&& other)
		{
			Set(move(other));
		}
		NoWeakPtr& operator=(NoWeakPtr<T, Deleter> const& other)
		{
			Set(other);
			return *this;
		}
		NoWeakPtr& operator=(NoWeakPtr<T, Deleter>&& other)
		{
			Set(move(other));
			return *this;
		}

		template <typename Y>
		NoWeakPtr(NoWeakPtr<Y, Deleter> const& other)
		{
			Set(other);
		}
		template <typename Y>
		NoWeakPtr(NoWeakPtr<Y, Deleter>&& other)
		{
			Set(move(other));
			return *this;
		}
		template <typename Y>
		NoWeakPtr& operator=(NoWeakPtr<Y, Deleter> const& other)
		{
			Set(other);
			return *this;
		}
		template <typename Y>
		NoWeakPtr& operator=(NoWeakPtr<Y, Deleter>&& other)
		{
			Set(move(other));
		}

		~NoWeakPtr()
		{
			Decrease();
		}
	private:
		void Decrease()
		{
			if (RC() == nullptr)
			{
				D()(p);
			}
			else
			{
				if (--*RC() == 0)
				{
					D()(p);
					delete RC();
				}
			}
		}

		template <typename Y>
		void Set(NoWeakPtr<Y, Deleter> const& other)
		{
			if (this != &other)
			{
				Decrease();
				p = other.p;
				rc = other.rc;
				if (other.p != nullptr)
				{
					if (RC() == nullptr)
					{
						CreateNewRC();
						other.rc.E1() = RC();
					}
					else
					{
						++*RC();
					}
				}
			}
		}

		template <typename Y>
		void Set(NoWeakPtr<Y, Deleter>&& other)
		{
			if (this != &other)
			{
				Decrease();
				p = other.p;
				rc = other.rc;
				other.p = nullptr;
				other.rc.E1() = nullptr;
			}
		}

		int* RC()
		{
			return rc.E1();
		}
		Deleter& D()
		{
			return rc.E2();
		}

		void CreateNewRC()
		{
			rc.E1() = new int(2);
		}

		T* p = nullptr;
		mutable CompressedPair<int*, Deleter> rc;
	};


	template <typename T, typename Deleter>
	struct ReferenceCountBase
	{
		using BaseReferencedType = T;
		using ReferenceCountBaseType = ReferenceCountBase<T, Deleter>;

		ReferenceCountBase() = default;
		ReferenceCountBase(Deleter& d)
			: rc(0, d)
		{
		}
		ReferenceCountBase(Deleter const& d)
			: rc(0, d)
		{
		}
		ReferenceCountBase(Deleter&& d)
			: rc(0, move(d))
		{
		}
		virtual ~ReferenceCountBase() = default;
	private:
		mutable CompressedPair<int, Deleter> rc;
		void Increase() const
		{
			rc.E1()++;
		}
		void Decrease() const
		{
			rc.E1()--;
			if (rc.E1() == 0)
			{
				rc.E2()(const_cast<T*>(static_cast<T const*>(this)));
			}
		}
		int ReferenceCount() const
		{
			return rc.E1();
		}
		template <typename Y>
		friend struct IntrusivePtr;
	};


	// for illustration purpose only
	// very bad implemention
	template <typename T>
	struct IntrusivePtr
	{
		T* p = nullptr;

		IntrusivePtr() = default;

		IntrusivePtr(nullptr_t)
		{
		}

		template <typename Y>
		explicit IntrusivePtr(Y* ptr)
		{
			assert(Cast(ptr)->ReferenceCount() == 0);
			p = ptr;
			if (p != nullptr)
			{
				Cast(p)->Increase();
			}
		}

		IntrusivePtr(IntrusivePtr const& other)
		{
			Set(other);
		}
		IntrusivePtr(IntrusivePtr&& other)
		{
			Set(move(other));
		}
		IntrusivePtr& operator=(IntrusivePtr const& other)
		{
			Set(other);
			return *this;
		}
		IntrusivePtr& operator=(IntrusivePtr&& other)
		{
			Set(move(other));
			return *this;
		}

		template <typename Y>
		IntrusivePtr(IntrusivePtr<Y> const& other)
		{
			Set(other);
		}
		template <typename Y>
		IntrusivePtr(IntrusivePtr<Y>&& other)
		{
			Set(move(other));
		}
		template <typename Y>
		IntrusivePtr& operator=(IntrusivePtr<Y> const& other)
		{
			Set(other);
			return *this;
		}
		template <typename Y>
		IntrusivePtr& operator=(IntrusivePtr<Y>&& other)
		{
			Set(move(other));
			return *this;
		}

		~IntrusivePtr()
		{
			Decrease();
		}

	private:
		template <typename Y>
		static typename T::ReferenceCountBaseType* Cast(Y* p)
		{
			return static_cast<typename T::ReferenceCountBaseType*>(p);
		}

		template <typename Y>
		void Set(IntrusivePtr<Y> const& other)
		{
			Decrease();
			p = other.p;
			if (p != nullptr)
			{
				Cast(p)->Increase();
			}
		}

		template <typename Y>
		void Set(IntrusivePtr<Y>&& other)
		{
			Decrease();
			p = other.p;
			other.p = nullptr;
		}

		void Decrease()
		{
			if (p != nullptr)
			{
				auto cp = Cast(p);
				cp->Decrease();
			}
		}
	};

	struct MemoryCheck
	{
		static int& InstanceCount()
		{
			static int ic = 0;
			return ic;
		}
		MemoryCheck()
		{
			InstanceCount()++;
		}
		~MemoryCheck()
		{
			InstanceCount()--;
		}
		MemoryCheck(MemoryCheck const&)
		{
			InstanceCount()++;
		}
		MemoryCheck& operator=(MemoryCheck const&)
		{
			InstanceCount()++;
			return *this;
		}
	};

	void CustomNoWeakPointer()
	{
		struct D
		{
			void operator()(int* p)
			{
				delete p;
			}
		};
		auto p = NoWeakPtr<int>(new int(1));
		auto pd = NoWeakPtr<int, D>(new int(42), D());
		decltype(pd) opd = pd;
		decltype(pd) ompd = move(pd);
		opd = pd;
		ompd = move(opd);
	}

	struct Resource;
	struct ResourceDeleter
	{
		int customData = 0;
		ResourceDeleter()
		{
		}
		ResourceDeleter(int cd)
			: customData(cd)
		{
		}
		void operator()(Resource* p);
	};
	
	struct Resource : ReferenceCountBase<Resource, ResourceDeleter>
	{
		Resource() = default;
		Resource(ResourceDeleter const& rd)
			: ReferenceCountBase<Resource, ResourceDeleter>(rd)
		{
		}
		MemoryCheck mc1;
	};

	struct ConcreteResource : Resource
	{
		using Resource::Resource;
		void F()
		{
#ifdef TurnOnCompileErrorCode
			Decrease(); // private
#endif
		}
		MemoryCheck mc2;
	};

	void ResourceDeleter::operator()(Resource* p)
	{
		delete p;
	}
	void CustomIntrusivePointer()
	{
		ConcreteResource r; // stack variable ok

		ResourceDeleter rd(42);

		auto p = IntrusivePtr<Resource>(new ConcreteResource(rd));
		auto pd = p;
		auto opd = move(pd);
		opd = pd;
	}

	void Run()
	{
		CustomNoWeakPointer();
		CustomIntrusivePointer();

		if (MemoryCheck::InstanceCount() != 0)
		{
			cout << "memory leaked!" << endl;
		}


		unique_ptr<int> up;
	}
}