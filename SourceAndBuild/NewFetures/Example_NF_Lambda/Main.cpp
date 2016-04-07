#include "LambdaBasic.h"
#include "Function.h"
#include <iostream>


int main()
{
	LambdaBasic::Run();
	Function::Run();

	std::cout << "press Enter to exit." << std::endl;
	std::cin.get();

}