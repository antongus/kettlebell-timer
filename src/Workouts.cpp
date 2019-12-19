/**
*  @file Workouts.cpp
*
*  Workouts class - set of workouts
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include <QJsonArray>

#include "Workouts.h"

QJsonValue Workouts::getJson()
{
	QJsonArray array;

	for (auto& workout : *this)
	{
		auto stepObj = workout.getJson().toObject();
		array.append(stepObj);
	}
	return array;
}

void Workouts::setJson(const QJsonValue& conf)
{
	clear();
//	addBuiltinWorkouts();

	for (auto stepVal : conf.toArray())
	{
		Workout workout;
		workout.setJson(stepVal.toObject());
		push_back(workout);
	}
}

std::optional<Workout> Workouts::find(const QString& title)
{
	auto findByTitle = [&](Workout const& w) { return title.compare(w.getTitle()) == 0; };
	if (auto it = std::find_if(begin(), end(), findByTitle); it != end())
		return *it;
	return {};
}

void Workouts::addBuiltinWorkouts()
{
	static constexpr char json1[] =
R"(
{
  "title": "Рывок 10 минут, темп 20",
  "steps": [
	{
	  "type": "pause",
	  "duration": 10000
	},
	{
	  "title": "Рывок!",
	  "type": "work",
	  "duration": 600000,
	  "attempts": 200
	}
  ]
}
)";

	Workout w1;
	w1.setJson(json1);
	push_back(w1);
}
