#include "AutomaticPointer.h"
#include "CustomizedPointer.h"
#include <iostream>


int main()
{
	AutomaticPointer::Run();
	CustomizedPointer::Run();

	std::cout << "press Enter to exit." << std::endl;
	std::cin.get();

}