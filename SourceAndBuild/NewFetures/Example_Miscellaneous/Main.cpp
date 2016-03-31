#include "DeleteAndDefault.h"
#include "TypeDeduction.h"
#include "RangeBasedForLoop.h"

int main()
{
	DeleteAndDefault::Run();
	TypeDeduction::Run();
	RangeBasedForLoop::Run();

	std::cout << "press any key to exit." << std::endl;
	std::cin.get();

}