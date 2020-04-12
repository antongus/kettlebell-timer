/**
*  @file ItemWithId.h
*
*  ItemWithId class - for creation of objects with unique ID
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

struct ItemWithId
{
	ItemWithId()
	{
		static int nextId { 1 };
		id = nextId++;
	}
	int getId() const { return id; }
private:
	int id;
};
