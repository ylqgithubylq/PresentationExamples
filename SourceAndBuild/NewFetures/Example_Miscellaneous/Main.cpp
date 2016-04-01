#include "DeleteAndDefault.h"
#include "Override.h"
#include "TypeDeduction.h"
#include "RangeBasedForLoop.h"

#include <iostream>

int main()
{
	DeleteAndDefault::Run();
	Override::Run();
	TypeDeduction::Run();
	RangeBasedForLoop::Run();

	std::cout << "press Enter to exit." << std::endl;
	std::cin.get();

}