/**
*  @file WorkoutEditor.cpp
*
*  Workouts editor
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include <QJsonDocument>
#include <QInputDialog>

#include "WorkoutEditor.h"

WorkoutEditor::WorkoutEditor(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);

	// create actions
	actionAddWorkout = new QAction(QIcon(":/icons/add_workout.svg"), tr("Add new workout"), this);
	connect(actionAddWorkout, &QAction::triggered, this, &WorkoutEditor::addWorkout);

	actionDeleteWorkout = new QAction(QIcon(":/icons/delete_workout.svg"), tr("Delete selected workout"), this);
	connect(actionDeleteWorkout, &QAction::triggered, this, &WorkoutEditor::deleteWorkout);

	actionAddStep = new QAction(QIcon(":/icons/plus.svg"), tr("Add workout step"), this);
	connect(actionAddStep, &QAction::triggered, this, &WorkoutEditor::addStep);

	actionDeleteStep = new QAction(QIcon(":/icons/process-stop.svg"), tr("Delete workout step"), this);
	connect(actionDeleteStep, &QAction::triggered, this, &WorkoutEditor::deleteStep);

	tbAddWorkout->setDefaultAction(actionAddWorkout);
	tbDeleteWorkout->setDefaultAction(actionDeleteWorkout);
	tbAddStep->setDefaultAction(actionAddStep);
	tbDeleteStep->setDefaultAction(actionDeleteStep);

	connect(listWorkouts, &QListWidget::currentItemChanged, this, &WorkoutEditor::selectedWorkoutChanged);
}

WorkoutEditor::~WorkoutEditor()
{
}

void WorkoutEditor::fill(const Workouts& w)
{
	workouts = w;
	listWorkouts->clear();
	for (auto& workout : workouts)
	{
		auto item = new QListWidgetItem(QIcon(":/icons/next.svg"), workout.getTitle(), listWorkouts);
		item->setData(Qt::UserRole, workout.getJson());
	}
}

void WorkoutEditor::addWorkout()
{
	auto ok { false };
	auto title = QInputDialog::getText(
	                 this,
	                 tr("Enter title for new workout"),
	                 tr("Title for new workout"),
	                 QLineEdit::Normal,
	                 "",
	                 &ok
	                 ).trimmed();
	if (!ok || title.isEmpty())
		return;
	Workout workout;
	workout.setTitle(title);
	workouts.push_back(workout);
	auto item = new QListWidgetItem(QIcon(":/icons/next.svg"), workout.getTitle(), listWorkouts);
	item->setData(Qt::UserRole, workout.getJson());
}

void WorkoutEditor::deleteWorkout()
{

}

void WorkoutEditor::addStep()
{

}

void WorkoutEditor::deleteStep()
{

}

void WorkoutEditor::selectedWorkoutChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
	if (current)
	{
		auto doc = QJsonDocument(current->data(Qt::UserRole).toJsonValue().toObject());
		edWorkout->setPlainText(doc.toJson());
	}
}
