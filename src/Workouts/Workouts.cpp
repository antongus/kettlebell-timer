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

	for (auto& workout : workouts)
	{
		auto stepObj = workout->getJson().toObject();
		array.append(stepObj);
	}
	return array;
}

void Workouts::setJson(const QJsonValue& conf)
{
	workouts.clear();
//	addBuiltinWorkouts();

	for (auto stepVal : conf.toArray())
	{
		auto& workout = workouts.emplace_back(std::make_shared<Workout>());
		workout->setJson(stepVal.toObject());
	}
}

std::shared_ptr<Workout> Workouts::find(int id)
{
	auto findById = [&](auto& w) { return id == w->getId(); };
	if (auto it = std::find_if(workouts.begin(), workouts.end(), findById); it != workouts.end())
		return *it;
	return nullptr;
}

std::shared_ptr<Workout> Workouts::find(const QString& title)
{
	auto findByTitle = [&](auto& w) { return title.compare(w->getTitle()) == 0; };
	if (auto it = std::find_if(workouts.begin(), workouts.end(), findByTitle); it != workouts.end())
		return *it;
	return nullptr;
}

std::shared_ptr<Workout> Workouts::add(const QString& title)
{
	auto workout = std::make_shared<Workout>();
	workouts.emplace_back(workout);
	workout->setTitle(title);
	return workout;
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

	auto& workout = workouts.emplace_back(std::make_shared<Workout>());
	workout->setJson(json1);
}
