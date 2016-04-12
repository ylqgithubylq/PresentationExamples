#pragma once
#include "chrono"

#define UseAssert 1

#if !UseAssert
#define NDEBUG
#endif

#define UseCompileErrorCode 0

#if UseCompileErrorCode
#define TurnOnCompileErrorCode
#endif // UseCompileErrorCode


template <typename C, typename D>
std::chrono::microseconds TimeDeltaInUs(std::chrono::time_point<C, D> start, std::chrono::time_point<C, D> end) noexcept
{
	auto d = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	return d;
}

inline std::chrono::time_point<std::chrono::high_resolution_clock> Now() noexcept
{
	return std::chrono::high_resolution_clock::now();
}

template <typename F>
auto MeasureExecutionTime(F& f) noexcept
{
	auto startTime = Now();
	f();
	auto endTime = Now();

	return TimeDeltaInUs(startTime, endTime);
}