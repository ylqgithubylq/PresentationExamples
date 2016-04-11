#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <cassert>
#include <array>
#include <iostream>

namespace ArrayView
{
	using namespace std;

	using byte = uint8_t;

	constexpr size_t DynamicSize = size_t(-1);
	
	template <typename T, size_t Count = DynamicSize>
	struct ArrayView
	{
		using Type = T;
		static size_t const ElementCount = Count;

		ArrayView() = default;
		ArrayView(ArrayView const&) = default;
		ArrayView(ArrayView&&) = default;
		ArrayView& operator=(ArrayView const&) = default;
		ArrayView& operator=(ArrayView&&) = default;

		template <typename Y>
		ArrayView(ArrayView<Y, ElementCount> const& other)
			: data(other.data)
		{
		}
		template <typename Y>
		ArrayView(ArrayView<Y, ElementCount>& other)
			: data(other.data)
		{
		}

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

		template<typename Y>
		ArrayView<Y> ViewAs()
		{
			return ArrayView<Y>(reinterpret_cast<Y*>(data), ElementCount * sizeof(T) / sizeof(Y));
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

		ArrayView() = default;
		ArrayView(ArrayView const&) = default;
		ArrayView(ArrayView&&) = default;
		ArrayView& operator=(ArrayView const&) = default;
		ArrayView& operator=(ArrayView&&) = default;

		template <typename Y>
		ArrayView(ArrayView<Y> const& other)
			: data(other.data), dataEnd(other.dataEnd)
		{
		}
		template <typename Y>
		ArrayView(ArrayView<Y>& other)
			: data(other.data), dataEnd(other.dataEnd)
		{
		}

		template <typename Y>
		ArrayView(Y* p, size_t count)
			: data(p), dataEnd(data + count)
		{
			assert(Count() >= 0);
		}

		template <typename Y, size_t N>
		ArrayView(Y(&a)[N])
			: data(a), dataEnd(data + N)
		{
		}
		template <typename Y, size_t N>
		ArrayView(Y const(&a)[N])
			: data(a), dataEnd(data + N)
		{
		}

		template <typename Container/*, typename = decltype(&Container::size), typename = decltype(&Container::data)*/>
		ArrayView(Container& c)
			: data(c.data()), dataEnd(data + c.size())
		{
		}
		template <typename Container/*, typename = decltype(&Container::size), typename = decltype(&Container::data)*/>
		ArrayView(Container const& c)
			: data(c.data()), dataEnd(data + c.size())
		{
		}

		size_t Count() const
		{
			return dataEnd - data;
		}

		T& operator[](size_t index)
		{
			assert(index < Count());
			return data[index];
		}

		T* GetData()
		{
			return data;
		}

		ArrayView<T> Front(size_t length)
		{
			assert(length <= Count());
			return ArrayView<T>(data, Count() - length);
		}
		ArrayView<T> Back(size_t length)
		{
			assert(length <= Count());
			return ArrayView<T>(data + length, Count() - length);
		}

		ArrayView<T> SubView(size_t start)
		{
			assert(start < Count());
			return ArrayView<T>(data + start, Count() - start);
		}
		ArrayView<T> SubView(size_t start, size_t length)
		{
			assert(start < Count());
			assert(start + length <= Count());
			return ArrayView<T>(data + start, length);
		}
		template<typename Y>
		ArrayView<Y> ViewAs()
		{
			return ArrayView<Y>(reinterpret_cast<Y*>(data), Count() * sizeof(T) / sizeof(Y));
		}

		T* begin()
		{
			return data;
		}
		T* end()
		{
			return dataEnd;
		}
		T const* begin() const
		{
			return data;
		}
		T const* end() const
		{
			return dataEnd;
		}

		T* data = nullptr;
		T* dataEnd = nullptr;
	};

	template <typename Container>
	ArrayView<typename Container::value_type> MakeView(Container& c)
	{
		return ArrayView<typename Container::value_type>(c.data(), c.size());
	}
	template <typename Container>
	ArrayView<typename Container::value_type const> MakeView(Container const& c)
	{
		return ArrayView<typename Container::value_type const>(c.data(), c.size());
	}

	template <typename T, size_t N0, size_t N1>
	void Copy(ArrayView<T const, N0> from, ArrayView<T, N1> to)
	{
		for (size_t i = 0; i < from.Count(); i++)
		{
			to[i] = from[i];
		}
	}



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

	int CalculateFirst(int i)
	{
		return i; // dummy implementation
	}
	int CalculateSecond(int i)
	{
		return i; // dummy implementation
	}
	int Convert(int i)
	{
		return i; // dummy implementation
	}

	void CalculateOld(vector<int> const& bases, int baseOffset, int count, int outputOffset, vector<pair<int, int>>& out)
	{
		for (int i = 0; i < count; i++)
		{ // calc 1st
			out[outputOffset + i].first = CalculateFirst(bases[baseOffset + i]);
		}
		for (int i = outputOffset; i < count + outputOffset; i++)
		{ // calc 2nd
			out[i].second = Convert(out[i].first);
		}
		for (int i = 0; i < count; i++)
		{ // acc 2nd
			out[outputOffset + i].second += CalculateSecond(bases[baseOffset + i]);
		}
	}


	void CalculateNew(ArrayView<int const> bases, ArrayView<pair<int, int>> out)
	{
		assert(bases.Count() == out.Count());
		size_t outCount = out.Count();
		for (size_t i = 0; i < outCount; i++)
		{
			out[i].first = CalculateFirst(bases[i]);
		}
		for (size_t i = 0; i < outCount; i++)
		{
			out[i].second = Convert(out[i].first);
		}
		for (size_t i = 0; i < outCount; i++)
		{
			out[i].second += CalculateSecond(bases[i]);
		}
	}


	void BoundAndPerformance()
	{
		assert(false);
		size_t VsCount = 100000;
		vector<int> vs;
		vs.resize(VsCount);
		for (size_t i = 0; i < VsCount; i++)
		{
			vs[i] = i;
		}
		vector<pair<int, int>> out;
		out.resize(VsCount);

		size_t baseOffset = 0;
		size_t count = VsCount;
		size_t outOffset = 0;

		auto oldTime = MeasureExecutionTime([&] ()
		{
			for (size_t i = 0; i < 10; i++)
			{
				CalculateOld(vs, baseOffset, count, outOffset, out);
			}
		});

		auto newTime = MeasureExecutionTime([&] ()
		{
			for (size_t i = 0; i < 10; i++)
			{
				auto vsView = MakeView(vs).SubView(baseOffset, count);
				auto outView = MakeView(out).SubView(outOffset, count);
				CalculateNew(vsView, outView);
			}
		});
		
		cout << "old time: " << oldTime.count() << ", new time: " << newTime.count() << endl;
	}


	struct Header
	{
		int c0; // something read from input
		int d0; // something calculated from c0
		int c1; // something read from input
		int d1; // something calculated from c1
	};
	struct Buffer
	{
		Header header;
		int bufferSize;
		byte* data;
		Buffer(int bufferSize)
			: bufferSize(bufferSize)
		{
			data = new byte[bufferSize];
		}
		~Buffer()
		{
			delete[] data;
		}
	};
	
	void ReadOld(void const* i, int n, Buffer* b)
	{
		if (!i || !b) return;
		byte const* p = (byte const*) i;

		memcpy(&b->header.c0, p, sizeof(int));
		p += sizeof(int);
		memcpy(&b->header.c1, p, sizeof(int));
		p += sizeof(int);

		b->header.d0 = b->header.c0 + 42;
		b->header.d1 = b->header.c1 + 42;

		memcpy(b->data, p, n);
	}

	void ReadNew(void const* i, int n, Buffer* b)
	{
		if (!i || !b) return;

		ArrayView<byte const> current(static_cast<byte const*>(i), n + 2 * sizeof(int));

		b->header.c0 = current.Front(sizeof(int)).ViewAs<int const>()[0];
		current = current.SubView(sizeof(int));
		b->header.c1 = current.Front(sizeof(int)).ViewAs<int const>()[0];
		current = current.SubView(sizeof(int));

		b->header.d0 = b->header.c0 + 42;
		b->header.d1 = b->header.c1 + 42;

		Copy(current, ArrayView<byte>(b->data, b->bufferSize));
	}




	void UnsafeRead()
	{
		array<int, 100> input;
		input.fill(42);
		Buffer b((input.size() - 2) * sizeof(int));
		ReadOld(input.data(), b.bufferSize, &b);
		ReadNew(input.data(), b.bufferSize, &b);
	}

	void Run()
	{
//		Example();
		BoundAndPerformance();
//		UnsafeRead();
// 		ArrayView<int const, 5> ad3 = ra;
	}
}