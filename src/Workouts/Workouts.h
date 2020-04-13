/**
*  @file Workout.h
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

#pragma once

#include <QString>
#include <QJsonObject>
#include <QAbstractListModel>
#include <vector>
#include <memory>

#include "Workout.h"

class Workouts : public QAbstractListModel
{
	Q_OBJECT

public:
	Workouts(QObject* parent = nullptr);
	~Workouts() override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	int rowCount(const QModelIndex &parent) const override;

	QJsonValue getJson();
	void setJson(const QJsonValue& conf);

	std::shared_ptr<Workout> getWorkout(size_t index);
	std::shared_ptr<Workout> find(int id);
	std::shared_ptr<Workout> add(QString const& title);
	void remove(int id);

	auto size() const { return workouts.size(); }
	auto begin() { return std::begin(workouts); }
	auto end() { return std::end(workouts); }
	auto begin() const { return std::begin(workouts); }
	auto end() const { return std::end(workouts); }

	void createDefaultWorkouts();

private:
	std::vector<std::shared_ptr<Workout>> workouts;
};
