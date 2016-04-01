#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <iostream>


namespace MoveConstructorAndAssignment
{
	using namespace std;

	struct Object
	{
		Object() = default;

		Object(Object&& other)
			: storage(move(other.storage))
		{
		}

		Object& operator=(Object&& other)
		{
			storage = move(other.storage);
			return *this;
		}

		string storage;
	};

	struct Detector
	{
		Detector() = default;
		~Detector()
		{
			cout << "~Detector()" << endl;
		}
		Detector(Detector const&)
		{
			cout << "Detector(Detector const&)" << endl;
		}
		Detector& operator=(Detector const&)
		{
			cout << "operator=(Detector const&)" << endl;
			return *this;
		}

		Detector(Detector&&)
		{
			cout << "Detector(Detector&&)" << endl;
		}
		Detector& operator=(Detector&&)
		{
			cout << "operator=(Detector&&)" << endl;
			return *this;
		}
		shared_ptr<int> dummy;
	};

	struct NoMove0
	{
		NoMove0() = default;
		NoMove0(NoMove0 const&)
			: detector(detector) // pretend this is a true copy
		{
		}
		Detector detector;
	};

	struct NoMove1
	{
		NoMove1() = default;
		~NoMove1()
		{
		}
		Detector detector;
	};

	struct NoCopy0
	{
		NoCopy0() = default;
		~NoCopy0()
		{
		}
		Detector detector;
	};

	void BasicMove()
	{
		Object object;
		Object otherObject0 = move(object); // call Object(Object&& other)
#if TurnOnCompileErrorCode
		Object otherObject1 = otherObject0;
#endif

		NoMove0 nm0;
		NoMove0 otherNM0 = move(nm0); // !! this will call NoMove0(NoMove0 const&) !!

		NoMove1 nm1;
		NoMove1 otherNM1 = move(nm1); // !! this will call NoMove1(NoMove1 const&) !!
	}


	struct MyVector
	{
		MyVector() = default;

		MyVector(MyVector&& other)
			: storage(other.storage)
			, size(other.size)
			, capacity(other.capacity)
		{
			other.storage = nullptr;
			other.size = 0;
			other.capacity = 0;
		}

		MyVector& operator=(MyVector&& other)
		{
			storage = other.storage;
			size = other.size;
			capacity = other.capacity;
			other.storage = nullptr;
			other.size = 0;
			other.capacity = 0;
			return *this;
		}

		int* storage = nullptr;
		int size = 0;
		int capacity = 0;
	};

	void WhatDoesVectorDo()
	{
		MyVector v;
		MyVector other = move(v);
	}

	void Run()
	{
		BasicMove();
		WhatDoesVectorDo();
	}
}