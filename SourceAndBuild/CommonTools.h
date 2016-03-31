#pragma once
#include "chrono"

#define UseCompileErrorCode 0

#if UseCompileErrorCode
#define TurnOnCompileErrorCode
#endif // UseCompileErrorCode


template <typename C, typename D>
std::chrono::microseconds TimeDeltaInUs(std::chrono::time_point<C, D> start, std::chrono::time_point<C, D> end)
{
	auto d = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	return d;
}

std::chrono::time_point<std::chrono::high_resolution_clock> Now()
{
	return std::chrono::high_resolution_clock::now();
}

template <typename F>
auto ExecutionTime(F& f)
{
	auto startTime = Now();
	f();
	auto endTime = Now();

	return TimeDeltaInUs(startTime, endTime);
}