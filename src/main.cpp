/**
*  @file main.cpp
*
*  Application main() function.
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include "Application.h"

int main(int argc, char *argv[])
{
	Application app(argc, argv);
	return app.run();
}

