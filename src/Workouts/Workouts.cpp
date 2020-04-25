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
#include <QIcon>

#include "Workouts.h"

Workouts::Workouts(QObject* parent)
    : QAbstractListModel(parent)
{
}

Workouts::~Workouts()
{

}

QVariant Workouts::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto row = static_cast<size_t>(index.row());
	if (row >= workouts.size())
		return QVariant();

	if (role == Qt::DisplayRole)
		return workouts[row]->getTitle();

	if (role == Qt::DecorationRole)
		return QIcon(":/icons/stopwatch.svg");

	if (role == Qt::UserRole)
		return workouts[static_cast<size_t>(index.row())]->getId();

	return QVariant();
}

int Workouts::rowCount(const QModelIndex& ) const
{
	return static_cast<int>(workouts.size());
}

QJsonValue Workouts::getJson()
{
	QJsonObject ret;

	QJsonArray array;
	for (auto& workout : workouts)
	{
		auto stepObj = workout->getJson().toObject();
		array.append(stepObj);
	}
	ret["workouts"] = array;

	return ret;
}

void Workouts::setJson(const QJsonValue& conf)
{
	beginResetModel();

	workouts.clear();

	auto obj = conf.toObject();
	for (auto stepVal : obj["workouts"].toArray())
	{
		auto workout = std::make_shared<Workout>();
		workouts.emplace_back(workout);
		workout->setJson(stepVal.toObject());
	}

	endResetModel();
}

std::shared_ptr<Workout> Workouts::getWorkout(size_t index)
{
	if (index < size())
		return workouts[index];
	return nullptr;
}

std::shared_ptr<Workout> Workouts::find(int id)
{
	auto findById = [&](auto& w) { return id == w->getId(); };
	auto it = std::find_if(workouts.begin(), workouts.end(), findById);
	if (it != workouts.end())
		return *it;
	return nullptr;
}

std::shared_ptr<Workout> Workouts::add(const QString& title)
{
	auto workout = std::make_shared<Workout>();
	auto endRow = static_cast<int>(workouts.size());
	beginInsertRows(QModelIndex(), endRow, endRow);
	workouts.emplace_back(workout);
	workout->setTitle(title);
	endInsertRows();

	return workout;
}

void Workouts::remove(int id)
{
	//dataChanged(index(indx), index(indx));
	auto findById = [&](auto& w) { return id == w->getId(); };
	auto end = std::end(workouts);
	beginRemoveRows(QModelIndex(), 0, static_cast<int>(workouts.size()));
	workouts.erase(std::remove_if(std::begin(workouts), end, findById), end);
	endRemoveRows();
}

void Workouts::createDefaultWorkouts()
{
	workouts.clear();
	auto workout = add(tr("Tabata"));
	auto step = workout->addStep(tr("Warmup"));
	step->setStartDelay(10);
	step->setRoundTime(60);
	step->setRoundAttempts(6);  // beep every 10 seconds
	step->setRoundCount(1);

	step = workout->addStep(tr("Tabata"));
	step->setStartDelay(10);
	step->setRoundTime(20);
	step->setRoundAttempts(4);
	step->setRoundCount(8);
	step->setRestTime(10);

}
