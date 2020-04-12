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

	actionAddStepWork = new QAction(QIcon(":/icons/play.svg"), tr("Add exersize to workout"), this);
	connect(actionAddStepWork, &QAction::triggered, this, &WorkoutEditor::addStep);

	actionDeleteStep = new QAction(QIcon(":/icons/process-stop.svg"), tr("Delete exersize from workout"), this);
	connect(actionDeleteStep, &QAction::triggered, this, &WorkoutEditor::deleteStep);

	// assign actions to buttons
	tbAddWorkout->setDefaultAction(actionAddWorkout);
	tbDeleteWorkout->setDefaultAction(actionDeleteWorkout);
	tbAddStepWork->setDefaultAction(actionAddStepWork);
	tbDeleteStep->setDefaultAction(actionDeleteStep);

	connect(listWorkouts, &QListWidget::currentItemChanged, this, &WorkoutEditor::selectedWorkoutChanged);
	connect(pbOk, &QPushButton::clicked, [=]{done(QDialog::Accepted); });
	connect(pbCancel, &QPushButton::clicked, [=]{done(QDialog::Rejected); });
}

WorkoutEditor::~WorkoutEditor()
{
}

QJsonValue WorkoutEditor::getJson()
{
	return workouts.getJson();
}

void WorkoutEditor::setJson(const QJsonValue& conf)
{
	workouts.setJson(conf);

	listWorkoutSteps->clear();
	listWorkouts->clear();
	for (auto& workout : workouts)
	{
		auto item = new QListWidgetItem(QIcon(":/icons/stopwatch.svg"), workout->getTitle(), listWorkouts);
		item->setData(Qt::UserRole, workout->getId());
		item->setData(Qt::UserRole + 1, workout->getJson());
	}
}

/**
 * get current (selected in left pane) workout
 */
std::shared_ptr<Workout> WorkoutEditor::getCurrentWorkout()
{
	auto item = listWorkouts->currentItem();
	if (item)
	{
		auto id = item->data(Qt::UserRole + 1).toInt();
		return workouts.find(id);
	}
	return nullptr;
}

/**
 * add new workout
 */
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

	auto workout = workouts.add(title);
	auto item = new QListWidgetItem(QIcon(":/icons/next.svg"), workout->getTitle(), listWorkouts);

	// store id and json into item data
	item->setData(Qt::UserRole, workout->getId());
	item->setData(Qt::UserRole + 1, workout->getJson());
}

void WorkoutEditor::deleteWorkout()
{
	auto item = listWorkouts->takeItem(listWorkouts->currentRow());
	if (item)
	{
//		auto id = item->data(Qt::UserRole + 1).toInt();
		delete item;
	}
}

void WorkoutEditor::addStep()
{
	auto workout = getCurrentWorkout();
	if (!workout)
		return;

	bool ok { false };
	auto title = QInputDialog::getText(
	                 this,
	                 tr("Enter title for new step"),
	                 tr("Title for new step"),
	                 QLineEdit::Normal,
	                 "",
	                 &ok
	                 ).trimmed();
	if (!ok || title.isEmpty())
		return;

	auto step = workout->addStep(title);
	auto item = new QListWidgetItem(QIcon(":/icons/next.svg"), workout->getTitle(), listWorkoutSteps);
	item->setData(Qt::UserRole, step->getId());
	item->setData(Qt::UserRole + 1, step->getJson());
}

void WorkoutEditor::deleteStep()
{

}

void WorkoutEditor::selectedWorkoutChanged(QListWidgetItem* current, QListWidgetItem* )
{
	if (current)
	{
		auto doc = QJsonDocument(current->data(Qt::UserRole + 1).toJsonValue().toObject());
		edWorkout->setPlainText(doc.toJson());
	}
}

void WorkoutEditor::selectedStepChanged(QListWidgetItem* current, QListWidgetItem* )
{
	if (current)
	{
		auto doc = QJsonDocument(current->data(Qt::UserRole + 1).toJsonValue().toObject());
		edWorkout->setPlainText(doc.toJson());
	}
}
