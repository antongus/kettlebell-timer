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
	static constexpr char caption[]            = "caption";
	static constexpr char initialDelay[]       = "initialDelay";
	static constexpr char duration[]           = "duration";
	static constexpr char attempts[]           = "attempts";
	static constexpr char attemptBeeps[]       = "attemptBeeps";
	static constexpr char loopCount[]          = "loopCount";
	static constexpr char loopPause[]          = "loopPause";
	static constexpr char pauseBeeps[]         = "pauseBeeps";
};

}


/**
 * get item configuration (as JSON)
 */
QJsonValue WorkoutStep::getJson() const
{
	QJsonObject obj;
	obj[VarNames::caption] = caption;
	obj[VarNames::initialDelay] = initialDelay;
	obj[VarNames::duration] = duration;
	obj[VarNames::attempts] = attempts;
	obj[VarNames::attemptBeeps] = attemptBeeps;
	obj[VarNames::loopCount] = loopCount;
	obj[VarNames::loopPause] = loopPause;
	obj[VarNames::pauseBeeps] = pauseBeeps;
	return obj;
}

/**
 * set item configuration (as JSON)
 */
void WorkoutStep::setJson(const QJsonValue& conf)
{
	auto obj = conf.toObject();
	caption        = obj[VarNames::caption].toString(caption);
	initialDelay   = obj[VarNames::initialDelay].toInt(10 * second);
	duration       = obj[VarNames::duration].toInt(10 * minute);
	attempts       = obj[VarNames::attempts].toInt(200);
	attemptBeeps   = obj[VarNames::attemptBeeps].toBool(true);
	loopCount      = obj[VarNames::loopCount].toInt(1);
	loopPause      = obj[VarNames::loopPause].toInt(5 * second);
	pauseBeeps     = obj[VarNames::pauseBeeps].toBool(true);
}
