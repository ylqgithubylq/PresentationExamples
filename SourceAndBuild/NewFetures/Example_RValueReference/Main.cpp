#include "RValueBasic.h"
#include "Move.h"
#include "MoveConstructorAndAssignment.h"
#include "RValueTemplate.h"
#include <iostream>


int main()
{
	RValueBasic::Run();
	Move::Run();
	MoveConstructorAndAssignment::Run();
	RValueTemplate::Run();
	std::cout << "press Enter to exit." << std::endl;
	std::cin.get();

}