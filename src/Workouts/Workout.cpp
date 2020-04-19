/**
*  @file Workout.cpp
*
*  Workout class - set or workout steps
*
*  Copyright 2020 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include <QJsonArray>
#include <QJsonObject>
#include <QtDebug>

#include "Workout.h"

Workout::Workout()
    : QObject(nullptr)
    , ItemWithId()
{
}

Workout::~Workout()
{
}

QJsonValue Workout::getJson() const
{
	QJsonObject ret;

	ret["title"] = title;

	QJsonArray array;
	for (auto& step : steps)
	{
		auto stepObj = step->getJson().toObject();
		array.append(stepObj);
	}
	ret["steps"] = array;

	return ret;
}

void Workout::setJson(const QJsonValue& conf)
{
	stop();

	auto obj = conf.toObject();
	title = obj["title"].toString(title);
	steps.clear();

	for (auto stepVal : obj["steps"].toArray())
	{
		auto stepPtr = std::make_shared<WorkoutStep>();
		stepPtr->setJson(stepVal.toObject());
		steps.push_back(stepPtr);
	}
}

std::shared_ptr<WorkoutStep> Workout::getStep(unsigned index)
{
	if (index >= steps.size())
		return nullptr;
	return steps[index];
}

std::shared_ptr<WorkoutStep> Workout::findStep(int id)
{
	auto findById = [&](auto& w) { return id == w->getId(); };
	if (auto it = std::find_if(steps.begin(), steps.end(), findById); it != steps.end())
		return *it;
	return nullptr;
}

std::shared_ptr<WorkoutStep> Workout::addStep(const QString& title)
{
	auto step = std::make_shared<WorkoutStep>();
	step->setCaption(title);
	steps.push_back(step);
	return step;
}

void Workout::swapSteps(int index1, int index2)
{
	auto begin = std::begin(steps);
	std::iter_swap(begin + index1, begin + index2);
}

void Workout::deleteStep(int id)
{
	auto findById = [&](auto& w) { return id == w->getId(); };
	auto end = std::end(steps);
	steps.erase(std::remove_if(std::begin(steps), end, findById), end);
}

/**
 * Start workout
 */
bool Workout::start()
{
	if (steps.empty())
	{
		return false;
	}
	running = true;
	emit started();
	currentStep = 0;
	emit attemptsChanged(0);
	emit timeChanged(0);
	return true;
}

/**
 * Stop workout
 */
void Workout::stop()
{
	if (running)
	{
		running = false;
		emit finished();
	}
}

void Workout::timer()
{

}
