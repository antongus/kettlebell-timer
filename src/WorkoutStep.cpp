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
 * get workout step configuration (as JSON)
 */
QJsonValue WorkoutStep::getJson()
{
	QJsonObject obj;
	obj["title"] = title;
	obj["type"] = getNameForType(type);
	obj["duration"] = duration;
	obj["beeps"] = beeps;
	return obj;
}

/**
 * set workout step configuration (as JSON)
 */
void WorkoutStep::setJson(const QJsonValue& conf)
{
	QJsonObject obj = conf.toObject();
	title = obj["title"].toString(title);
	duration = obj["duration"].toInt(1000);
	beeps = obj["beeps"].toInt(0);
	type = getTypeFromName(obj["type"].toString());
}

/**
 * get name for given workout type
 */
QString WorkoutStep::getNameForType(Type t)
{
	switch (t)
	{
	case Type::Comment:
		return "comment";
	case Type::Pause:
		return "pause";
	case Type::Work:
		return "work";
	case Type::Message:
		return "message";
	case Type::Sound:
		return "sound";
	}
	return "unknown";
}

/**
 * Get workout type from it name
 */
WorkoutStep::Type WorkoutStep::getTypeFromName(QString name)
{
	name = name.trimmed().toLower();
	if (name.compare("pause") == 0)
		return Type::Pause;
	if (name.compare("work") == 0)
		return Type::Work;
	if (name.compare("message") == 0)
		return Type::Message;
	if (name.compare("sound") == 0)
		return Type::Sound;

	// default type: comment
	return Type::Comment;
}
