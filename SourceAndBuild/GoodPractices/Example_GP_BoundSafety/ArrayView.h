#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <tuple>
#include <algorithm>
#include <cassert>
#include <array>
#include <cmath>

namespace ArrayView
{
	using namespace std;

	constexpr size_t DynamicSize = size_t(-1);
	
	template <typename T, size_t Count = DynamicSize>
	struct ArrayView
	{
		using Type = T;
		static size_t const ElementCount = Count;

		template <typename Y, size_t N>
		ArrayView(Y(&a)[N])
			: data(a)
		{
		}
		template <typename Y, size_t N>
		ArrayView(Y const(&a)[N])
			: data(a)
		{
		}

		template <typename Y, size_t N>
		ArrayView(array<Y, N>& a)
			: data(a.data())
		{
		}
		template <typename Y, size_t N>
		ArrayView(array<Y, N> const& a)
			: data(a.data())
		{
		}


		size_t Count() const
		{
			return ElementCount;
		}

		T& operator[](size_t index)
		{
			assert(index < ElementCount);
			return data[index];
		}

		T* GetData()
		{
			return data;
		}

		ArrayView<T> SubView(size_t start)
		{
			assert(start < ElementCount);
			return ArrayView<T>(data + start, ElementCount - start);
		}
		ArrayView<T> SubView(size_t start, size_t length)
		{
			assert(start < ElementCount);
			assert(start + length <= ElementCount);
			return ArrayView<T>(data + start, length);
		}

		T* begin()
		{
			return data;
		}
		T* end()
		{
			return data + ElementCount;
		}
		T const* begin() const
		{
			return data;
		}
		T const* end() const
		{
			return data + ElementCount;
		}

		T* data = nullptr;
	};

	template <typename T, size_t N>
	auto MakeStaticView(T(&a)[N])
	{
		return ArrayView<T, N>(a);
	}

	template <typename T, size_t N>
	auto MakeStaticView(array<T, N>& a)
	{
		return ArrayView<T, N>(a);
	}
	template <typename T, size_t N>
	auto MakeStaticView(array<T, N> const& a)
	{
		return ArrayView<const T, N>(a);
	}

	template <typename T>
	struct ArrayView<T, DynamicSize>
	{
		using Type = T;
		static size_t const ElementCount = DynamicSize;

		template <typename Y, size_t N>
		ArrayView(Y(&a)[N])
			: data(a), count(N)
		{
		}
		template <typename Y, size_t N>
		ArrayView(Y const(&a)[N])
			: data(a), count(N)
		{
		}

		template <typename Container>
		ArrayView(Container& c)
			: data(c.data()), count(c.size())
		{
		}
		template <typename Container>
		ArrayView(Container const& c)
			: data(c.data()), count(c.size())
		{
		}

		size_t Count() const
		{
			return count;
		}

		T& operator[](size_t index)
		{
			assert(index < count);
			return data[index];
		}

		T* GetData()
		{
			return data;
		}

		ArrayView<T> SubView(size_t start)
		{
			assert(start < count);
			return ArrayView<T>(data + start, count - start);
		}
		ArrayView<T> SubView(size_t start, size_t length)
		{
			assert(start < count);
			assert(start + length <= count);
			return ArrayView<T>(data + start, length);
		}

		T* begin()
		{
			return data;
		}
		T* end()
		{
			return data + count;
		}
		T const* begin() const
		{
			return data;
		}
		T const* end() const
		{
			return data + count;
		}

		T* data = nullptr;
		size_t count = 0;
	};

	void F(ArrayView<int const> data)
	{
		for (int i : data)
		{
			i = i; // use
		}
	}

	template <size_t N>
	void SF(ArrayView<int const, N> data)
	{
		for (int i : data)
		{
			i = i; // use
		}
	}

	void Example()
	{
		int const ra[4] = { 1, 2, 3, 4 };
		array<int, 4> const aa = { 4, 3, 2, 1 };

		F(ra);
		F(aa);
		SF(MakeStaticView(ra));
		SF(MakeStaticView(aa));
	}
	void Test1()
	{

	}

	void Run()
	{
		Example();
		Test1();
// 		ArrayView<int const, 5> ad3 = ra;
	}
}