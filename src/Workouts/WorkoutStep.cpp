/**
*  @file WorkoutStep.cpp
*
*  WorkoutStep class - single step of workout
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include "WorkoutStep.h"

/**
 * get item configuration (as JSON)
 */
QJsonValue WorkoutStep::getJson() const
{
	QJsonObject obj;
	obj["caption"] = caption;
	obj["delayBeforeStart"] = delayBeforeStart;
	obj["duration"] = duration;
	obj["repeatCount"] = repeatCount;
	obj["pauseBetweenRepeats"] = pauseBetweenRepeats;
	return obj;
}

/**
 * set item configuration (as JSON)
 */
void WorkoutStep::setJson(const QJsonValue& conf)
{
	auto obj = conf.toObject();
	caption = obj["caption"].toString(caption);
	delayBeforeStart = obj["delayBeforeStart"].toInt(10 * second);
	duration = obj["duration"].toInt(20 * minute);
	repeatCount = obj["repeatCount"].toInt(1);
	pauseBetweenRepeats = obj["pauseBetweenRepeats"].toInt(5 * second);
}
