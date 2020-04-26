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

static constexpr char VarName_caption[]          = "caption";
static constexpr char VarName_startDelay[]       = "startDelay";
static constexpr char VarName_roundTime[]        = "roundTime";
static constexpr char VarName_roundAttempts[]    = "roundAttempts";
static constexpr char VarName_roundCount[]       = "roundCount";
static constexpr char VarName_restTime[]         = "restTime";


/**
 * get item configuration (as JSON)
 */
QJsonValue WorkoutStep::getJson() const
{
	QJsonObject obj;
	obj[VarName_caption]       = caption;
	obj[VarName_startDelay]    = startDelay;
	obj[VarName_roundTime]     = roundTime;
	obj[VarName_roundAttempts] = roundAttempts;
	obj[VarName_roundCount]    = roundCount;
	obj[VarName_restTime]      = restTime;
	return obj;
}

/**
 * set item configuration (as JSON)
 */
void WorkoutStep::setJson(const QJsonValue& conf)
{
	auto obj = conf.toObject();
	caption        = obj[VarName_caption].toString(caption);
	startDelay     = obj[VarName_startDelay].toInt(10 * second);
	roundTime      = obj[VarName_roundTime].toInt(10 * minute);
	roundAttempts  = obj[VarName_roundAttempts].toInt(200);
	roundCount     = obj[VarName_roundCount].toInt(1);
	restTime       = obj[VarName_restTime].toInt(5 * second);
}
