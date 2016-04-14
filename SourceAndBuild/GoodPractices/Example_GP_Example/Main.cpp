#include "LinearContainer.h"
#include "UndoRedo.h"
#include <iostream>

int main()
{
	LinearContainer::Run();
	UndoRedo::Run();

	std::cout << "press Enter to exit." << std::endl;
	std::cin.get();

}