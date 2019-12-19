/**
*  @file Workout.cpp
*
*  Workout class - set or workout steps
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include <QJsonArray>

#include "Workout.h"

QJsonValue Workout::getJson()
{
	QJsonObject ret;

	ret["title"] = title;

	QJsonArray array;
	for (auto& step : steps)
	{
		auto stepObj = step.getJson().toObject();
		array.append(stepObj);
	}
	ret["steps"] = array;

	return ret;
}

void Workout::setJson(const QJsonValue& conf)
{
	auto obj = conf.toObject();

	title = obj["title"].toString(title);

	steps.clear();
	for (auto stepVal : obj["steps"].toArray())
	{
		WorkoutStep step;
		step.setJson(stepVal.toObject());
		steps.push_back(step);
	}
}

std::optional<WorkoutStep> Workout::getCurrentStep()
{
	if (pos < steps.size())
		return steps[pos];
	return {};
}
