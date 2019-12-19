/**
*  @file Workout.h
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

#pragma once

#include <QString>
#include <QJsonObject>
#include <vector>
#include <optional>

#include "WorkoutStep.h"

class Workout
{
public:
	QJsonValue getJson();
	void setJson(const QJsonValue& conf);

	void setTitle(QString value) { title = std::move(value); }
	QString const& getTitle() const { return title; }

	void start() { pos = 0; }
	void next() { ++pos; }
	std::optional<WorkoutStep> getCurrentStep();

	auto begin() { return std::begin(steps); }
	auto begin() const { return std::begin(steps); }
	auto end() { return std::end(steps); }
	auto end() const { return std::end(steps); }
private:
	QString title;
	std::vector<WorkoutStep> steps;
	std::size_t pos { 0 };
};
