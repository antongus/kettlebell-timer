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

namespace  {
/// struct for holding variable names
struct VarNames
{
	static constexpr char caption[]          = "caption";
	static constexpr char startDelay[]       = "startDelay";
	static constexpr char roundTime[]        = "roundTime";
	static constexpr char roundAttempts[]    = "roundAttempts";
	static constexpr char roundCount[]       = "roundCount";
	static constexpr char restTime[]         = "restTime";
};

}


/**
 * get item configuration (as JSON)
 */
QJsonValue WorkoutStep::getJson() const
{
	QJsonObject obj;
	obj[VarNames::caption]       = caption;
	obj[VarNames::startDelay]    = startDelay;
	obj[VarNames::roundTime]     = roundTime;
	obj[VarNames::roundAttempts] = roundAttempts;
	obj[VarNames::roundCount]    = roundCount;
	obj[VarNames::restTime]      = restTime;
	return obj;
}

/**
 * set item configuration (as JSON)
 */
void WorkoutStep::setJson(const QJsonValue& conf)
{
	auto obj = conf.toObject();
	caption        = obj[VarNames::caption].toString(caption);
	startDelay     = obj[VarNames::startDelay].toInt(10 * second);
	roundTime      = obj[VarNames::roundTime].toInt(10 * minute);
	roundAttempts  = obj[VarNames::roundAttempts].toInt(200);
	roundCount     = obj[VarNames::roundCount].toInt(1);
	restTime       = obj[VarNames::restTime].toInt(5 * second);
}
