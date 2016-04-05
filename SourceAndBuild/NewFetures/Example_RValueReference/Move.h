#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <string>
#include <cassert>

namespace Move
{
	using namespace std;

	struct RVFunctionExample
	{
		void RValueMethod(string&& s)
		{
			storage = move(s); // s is an lvalue
		}

		string storage;
	};

	void Run()
	{
		vector<string> vs;
		string s;
		vs.push_back(move(s)); // call (string&&)
		assert(s.empty());

	}
}