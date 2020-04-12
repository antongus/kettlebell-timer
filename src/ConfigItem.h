/**
*  @file ConfigItem.h
*
*  Item with JSON configuration (abstract class)
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

#include <QJsonValue>

class ConfigItem
{
public:
	ConfigItem() = default;
	virtual ~ConfigItem() = default;

	virtual QJsonValue getJson() const = 0;
	virtual void setJson(QJsonValue const& conf) = 0;
};
