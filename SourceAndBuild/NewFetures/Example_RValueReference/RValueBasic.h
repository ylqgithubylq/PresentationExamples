#pragma once
#include "CommonTools.h"

#include <utility>
#include <vector>
#include <string>

namespace RValueBasic
{
	using namespace std;

	void Basic()
	{
		vector<string> vs;
		string lvs("this is a lvalue string");
		vs.push_back(lvs);
		vs.push_back(string("this is a rvalue string"));
	}

	struct RVFunctionExample
	{
		void RValueMethod(string&& s)
		{
			swap(storage, s); // s is a lvalue
			s.clear();
		}

		void LValueMethod(string const& s)
		{
			storage = s; // copy
		}
		string storage;
	};
	string CreateString()
	{
		return string("created");
	}

	void RValueVariable()
	{
		string&& rrs = string("rvalue");
		RVFunctionExample example;
		example.LValueMethod(rrs);
#ifdef TurnOnCompileErrorCode
		example.RValueMethod(rrs); // error
#endif
		string&& created = CreateString();
		created += "can rvalue referenced object be used like this?";
	}

	void Run()
	{
		RValueVariable();
	}
}