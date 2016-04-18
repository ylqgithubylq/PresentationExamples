#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include <array>
#include <algorithm>
#include <iostream>

namespace LinearContainer
{
	using namespace std;

	template <typename T>
	struct List
	{
		List() noexcept = default;

		List(List const& other)
		{
			Add_Uninitialized(other.size);
			UninitializedCopyElements(ptr, other.ptr, other.size);
		}

		List(List&& other) noexcept
		{
			ptr = other.ptr;
			size = other.size;
			capacity = other.capacity;
			other.ptr = nullptr;
			other.size = 0;
			other.capacity = 0;
		}

		List& operator=(List const& other)
		{
			if (this != &other)
			{
				Clear();
				Add_Uninitialized(other.size);
				UninitializedCopyElements(ptr, other.ptr, other.size);
			}
			return *this;
		}

		List& operator=(List&& other) noexcept
		{
			if (this != &other)
			{
				DestructThis();
				ptr = other.ptr;
				size = other.size;
				capacity = other.capacity;
				other.ptr = nullptr;
				other.size = 0;
				other.capacity = 0;
			}
			return *this;
		}

		~List()
		{
			DestructThis();
		}

		void Add(T const& e)
		{
			Emplace(e);
		}

		void Add(T&& e)
		{
			Emplace(move(e));
		}

		template <typename... Parameters>
		void Emplace(Parameters&&... parameters)
		{
			GrowIfNeedAdd1();
			UninitializedConstructElement(ptr + size, forward<Parameters>(parameters)...);
			size += 1;
		}

		void Add_Uninitialized(uint32_t count)
		{
			GrowIfNeed(count);
			size += count;
		}

		void RemoveLast() noexcept
		{
			DestructElement(ptr + size - 1);
			size -= 1;
		}

		T GetAndRemoveLast() noexcept(noexcept(move(ptr[0])))
		{
			T toReturn = move(ptr[size - 1]);
			RemoveLast();
			return toReturn;
		}

		void SetSize(uint32_t newSize)
		{
			uint32_t oldSize = size;
			SetSizeUninitialized(newSize);
			if (newSize > oldSize)
			{
				DefaultConstructElements(ptr + oldSize, newSize - oldSize);
			}
		}

		void SetSize_Uninitialized(uint32_t newSize)
		{
			if (newSize <= size)
			{
				DestructElements(ptr + newSize, size - newSize);
			}
			else if (newSize > capacity)
			{
				ReallocateStorage(newSize);
			}
			size = newSize;
		}

		void Clear() noexcept
		{
			DestructElements(ptr, size);
			size = 0;
		}

	private:

		void GrowIfNeedAdd1()
		{
			if (size == capacity)
			{
				uint32_t nextCapacity = GetNextCapacity(capacity);
				ReallocateStorage(nextCapacity);
			}
		}

		void GrowIfNeed(uint32_t count)
		{
			uint32_t newCount = count + size;
			if (newCount > capacity)
			{
				uint32_t nextCapacity = max(GetNextCapacity(capacity), newCount);
				ReallocateStorage(nextCapacity);
			}
		}

		void ReallocateStorage(uint32_t newCapacity)
		{
			T* newPtr = nullptr;
			if (newCapacity > 0 && newCapacity != capacity)
			{
				newPtr = AllocateMemory(newCapacity);
				uint32_t moveElementCount = min(size, newCapacity);
				if (size > 0)
				{
					try
					{
						UninitializedMoveElements(newPtr, ptr, moveElementCount);
						DestructElements(ptr, size);
					}
					catch (...)
					{
						ReleaseMemory(newPtr);
						throw;
					}
				}
			}
			if (newCapacity != capacity)
			{
				ReleaseMemory(ptr);
				ptr = newPtr;
				capacity = newCapacity;
			}
		}

		void DestructThis() noexcept
		{
			if (ptr != nullptr)
			{
				DestructElements(ptr, size);
				ReleaseMemory(ptr);
				ptr = nullptr;
				size = 0;
				capacity = 0;
			}
		}

		static uint32_t GetNextCapacity(uint32_t oldCapacity) noexcept
		{
			if (oldCapacity < 2)
			{
				return 4;
			}
			else
			{
				return oldCapacity + oldCapacity / 2 + 2; // 1.5x + 2
			}
		}

		static void DefaultConstructElements(T* p, uint32_t count) noexcept(noexcept(T()))
		{
			for (uint32_t i = 0; i < count; i++)
			{
				new (static_cast<void*>(p + i)) T();
			}
		}

		template <typename... Parameters>
		static void UninitializedConstructElement(T* p, Parameters&&... parameters) noexcept(noexcept(T(forward<Parameters>(parameters)...)))
		{
			new (static_cast<void*>(p)) T(forward<Parameters>(parameters)...);
		}

		static void DestructElement(T* p) noexcept
		{
			p->~T();
		}
		static void DestructElements(T* p, uint32_t count) noexcept
		{
			for (uint32_t i = 0; i < count; i++)
			{
				DestructElement(p + i);
			}
		}

		template <typename F>
		static void GuardedLoop(T* to, uint32_t count, F f) noexcept(noexcept(f(uint32_t(0))))
		{
			uint32_t done = 0;
			try
			{
				for (uint32_t i = 0; i < count; i++, done++)
				{
					f(i);
				}
			}
			catch (const std::exception&)
			{
				for (uint32_t i = 0; i < done; i++)
				{
					DestructElement(to + i);
				}
				throw;
			}
		}

		static void MoveElements(T* to, T* from, uint32_t count) noexcept(noexcept(to[0] = move(from[0])))
		{
			GuardedLoop(to, count, [to, from] (uint32_t i)
			{
				to[i] = move(from[i]);
			});
		}

		static void CopyElements(T* to, T* from, uint32_t count) noexcept(noexcept(to[0] = from[0]))
		{
			GuardedLoop(to, count, [to, from] (uint32_t i)
			{
				to[i] = from[i];
			});
		}

		static void UninitializedMoveElements(T* to, T* from, uint32_t count) noexcept(noexcept(UninitializedConstructElement(to, move(from[0]))))
		{
			GuardedLoop(to, count, [to, from] (uint32_t i)
			{
				UninitializedConstructElement(to + i, move(from[i]));
			});
		}

		static void UninitializedCopyElements(T* to, T* from, uint32_t count) noexcept(noexcept(UninitializedConstructElement(to, move(from[0]))))
		{
			GuardedLoop(to, count, [to, from] (uint32_t i)
			{
				UninitializedConstructElement(to + i, from[i]);
			});
		}

		static T* AllocateMemory(uint32_t elementCount)
		{
			return static_cast<T*>(malloc(elementCount * sizeof(T)));
		}

		static void ReleaseMemory(T* p) noexcept
		{
			free(p);
		}

		T* ptr = nullptr;
		uint32_t size = 0;
		uint32_t capacity = 0;
	};

	struct MemoryCheck
	{
		static int& InstanceCount() noexcept
		{
			static int ic = 0;
			return ic;
		}
		MemoryCheck() noexcept
		{
			InstanceCount()++;
		}
		~MemoryCheck()
		{
			InstanceCount()--;
		}
		MemoryCheck(MemoryCheck const&) noexcept
		{
			InstanceCount()++;
		}
		MemoryCheck& operator=(MemoryCheck const&) noexcept
		{
			InstanceCount()++;
			return *this;
		}
	};


	void ListTest()
	{
		List<MemoryCheck> l1;

		l1.Add(MemoryCheck());

		l1 = List<MemoryCheck>();

		List<MemoryCheck> l2;

		l1.Emplace();
		l2 = l1;

		auto mc = l1.GetAndRemoveLast();
	}

	void Run()
	{
		ListTest();

		if (MemoryCheck::InstanceCount() != 0)
		{
			cout << "memory leaked!" << endl;
		}
	}
}