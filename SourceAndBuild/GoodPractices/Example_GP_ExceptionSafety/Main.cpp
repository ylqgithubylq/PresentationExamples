#include "PerfomanceComparison.h"
#include "ErrorHandling.h"
#include <iostream>

int main()
{
	PerfomanceComparison::Run();
	ErrorHandling::Run();

	std::cout << "press Enter to exit." << std::endl;
	std::cin.get();

}