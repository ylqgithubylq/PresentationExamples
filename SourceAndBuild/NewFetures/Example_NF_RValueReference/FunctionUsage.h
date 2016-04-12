#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <array>
#include <string>
#include <cassert>
#include <iostream>

namespace FunctionUsage
{
	using namespace std;

	constexpr int DataSize = 1024 * 32;
	constexpr int HeapAllocDataSize = 1024 * 32;

	struct BigPOD
	{
		array<float, DataSize> data;
		float clearValue;

		BigPOD()
		{
			// uninitialized
		}

		BigPOD(float clearValue)
			: clearValue(clearValue)
		{
		}

		~BigPOD()
		{
		}

		void Clear()
		{
			data.fill(clearValue);
		}
	};

	BigPOD CreateBig(int x)
	{
		BigPOD big(3.14f * x);
		big.Clear();
		return big;
	}

	void CreateBigTo(BigPOD& big, int x)
	{
		big = BigPOD(3.14f * x);
		big.Clear();
	}

	void ModifyBig(BigPOD& big, int x)
	{
		big.clearValue = 3.14f * x;
		big.Clear();
	}

	void POD()
	{
		auto count = make_unique<int>(10000);

		auto dummy0 = make_unique<float>(0.f);
		auto dummy1 = make_unique<float>(0.f);

		auto returnValueTime = MeasureExecutionTime([&]
		{
			for (int i = 0; i < *count; i++)
			{
				BigPOD big = CreateBig(i);
				*dummy0 += big.data[0];
			}
		});

		auto outParameterTime = MeasureExecutionTime([&]
		{
			for (int i = 0; i < *count; i++)
			{
				BigPOD big;
				CreateBigTo(big, i);
				*dummy1 += big.data[0];
			}
		});

		auto modifyTime = MeasureExecutionTime([&]
		{
			for (int i = 0; i < *count; i++)
			{
				BigPOD big(3.14f);
				ModifyBig(big, i);
				*dummy1 += big.data[0];
			}
		});

		cout << "return value time: " << returnValueTime.count() << ", out parameter time: " << outParameterTime.count() << ", modify time: " << modifyTime.count() << endl;
	}

	vector<float> CreateContainer(int x)
	{
		vector<float> container;
		container.resize(HeapAllocDataSize, 3.14f * x);
		return container;
	}

	void CreateContainerTo(vector<float>& container, int x)
	{
		container.clear();
		container.resize(HeapAllocDataSize, 3.14f * x);
	}

	void Container()
	{
		auto count = make_unique<int>(10000);

		auto dummy0 = make_unique<float>(0.f);
		auto dummy1 = make_unique<float>(0.f);
		auto dummy2 = make_unique<float>(0.f);
		auto dummy3 = make_unique<float>(0.f);

		auto returnValueTime = MeasureExecutionTime([&]
		{
			for (int i = 0; i < *count; i++)
			{
				vector<float> big = CreateContainer(i);
				*dummy0 += big[0];
			}
		});

		auto outParameterTime = MeasureExecutionTime([&]
		{
			for (int i = 0; i < *count; i++)
			{
				vector<float> big;
				CreateContainerTo(big, i);
				*dummy1 += big[0];
			}
		});

		auto returnValueReuseTime = MeasureExecutionTime([&]
		{
			vector<float> big;
			for (int i = 0; i < *count; i++)
			{
				big = CreateContainer(i);
				*dummy2 += big[0];
			}
		});

		auto outParameterReuseTime = MeasureExecutionTime([&]
		{
			vector<float> big;
			for (int i = 0; i < *count; i++)
			{
				CreateContainerTo(big, i);
				*dummy3 += big[0];
			}
		});

		cout << "return value time: " << returnValueTime.count() << ", out parameter time: " << outParameterTime.count() << endl;
		cout << "return value reuse time: " << returnValueReuseTime.count() << ", out parameter reuse time: " << outParameterReuseTime.count() << endl;
	}


	void Run()
	{
		POD();
		Container();
	}
}