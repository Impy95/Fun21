/*
*@author: Vaughn Rowse
*@file: Command.cpp
*@description: Commands objects that are loaded into a queue
*/
#include "Command.h"
#include "Category.h"

namespace GEX {
	Command::Command():
		action(),
		category(Category::None)
	{
	}

}
