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

	// Ok and Cancel buttons
	connect(pbOk, &QPushButton::clicked, [=]{done(QDialog::Accepted); });
	connect(pbCancel, &QPushButton::clicked, [=]{done(QDialog::Rejected); });

	// create actions
	actionAddWorkout = new QAction(QIcon(":/icons/add_workout.svg"), tr("Add new workout"), this);
	connect(actionAddWorkout, &QAction::triggered, this, &WorkoutEditor::addWorkout);

	actionDeleteWorkout = new QAction(QIcon(":/icons/delete_workout.svg"), tr("Delete selected workout"), this);
	connect(actionDeleteWorkout, &QAction::triggered, this, &WorkoutEditor::deleteWorkout);

	actionAddStepWork = new QAction(QIcon(":/icons/add_step.svg"), tr("Add step to workout"), this);
	connect(actionAddStepWork, &QAction::triggered, this, &WorkoutEditor::addStep);

	actionDeleteStep = new QAction(QIcon(":/icons/delete_step.svg"), tr("Delete step from workout"), this);
	connect(actionDeleteStep, &QAction::triggered, this, &WorkoutEditor::deleteStep);

	// assign actions to buttons
	tbAddWorkout->setDefaultAction(actionAddWorkout);
	tbDeleteWorkout->setDefaultAction(actionDeleteWorkout);
	tbAddStepWork->setDefaultAction(actionAddStepWork);
	tbDeleteStep->setDefaultAction(actionDeleteStep);

	loadWorkoutStep(nullptr);

	connect(listWorkouts, &QListWidget::currentItemChanged, this, &WorkoutEditor::selectedWorkoutChanged);
	connect(listWorkoutSteps, &QListWidget::currentItemChanged, this, &WorkoutEditor::selectedWorkoutStepChanged);
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
		item->setData(idRole, workout->getId());
	}
	listWorkouts->setCurrentRow(0);
}

/**
 * get current (selected in left pane) workout
 */
std::shared_ptr<Workout> WorkoutEditor::getSelectedWorkout()
{
	auto item = listWorkouts->currentItem();
	if (item)
	{
		auto id = item->data(idRole).toInt();
		return workouts.find(id);
	}
	return nullptr;
}

std::shared_ptr<WorkoutStep> WorkoutEditor::getSelectedWorkoutStep()
{
	auto currentWorkout = getSelectedWorkout();
	if (currentWorkout)
	{
		auto item = listWorkoutSteps->currentItem();
		if (item)
		{
			auto id = item->data(idRole).toInt();
			return currentWorkout->findStep(id);
		}
	}
	return nullptr;
}

/**
 * Load workout step to editor
 */
void WorkoutEditor::loadWorkoutStep(std::shared_ptr<WorkoutStep> step)
{
	// save current step
	if (currentWorkoutStep)
	{
		currentWorkoutStep->setCaption(edStepCaption->text());
		currentWorkoutStep->setDelayBeforeStart(sbDelayBeforeStart->value());
	}
	currentWorkoutStep = step;
	auto const hasStep = currentWorkoutStep != nullptr;
	edStepCaption->setEnabled(hasStep);
	sbDelayBeforeStart->setEnabled(hasStep);
	sbStepDuration->setEnabled(hasStep);
	sbStepRepeatCount->setEnabled(hasStep);
	sbPauseBetweenRepeats->setEnabled(hasStep);

	if (hasStep)
	{
		edStepCaption->setText(currentWorkoutStep->getCaption());
		sbDelayBeforeStart->setValue(currentWorkoutStep->getDelayBeforeStart());
		sbStepDuration->setValue(currentWorkoutStep->getDuration());
		sbStepRepeatCount->setValue(currentWorkoutStep->getRepeatCount());
		sbPauseBetweenRepeats->setValue(currentWorkoutStep->getPauseBetweenRepeats());
	}
	else
	{
		edStepCaption->setText("");
		sbDelayBeforeStart->setValue(0);
		sbStepDuration->setValue(0);
		sbStepRepeatCount->setValue(0);
		sbPauseBetweenRepeats->setValue(0);
	}
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
	auto item = new QListWidgetItem(QIcon(":/icons/stopwatch.svg"), workout->getTitle(), listWorkouts);

	// store id and json into item data
	item->setData(idRole, workout->getId());
	listWorkouts->setCurrentItem(item);
}

void WorkoutEditor::deleteWorkout()
{
	auto item = listWorkouts->takeItem(listWorkouts->currentRow());
	if (item)
	{
		auto id = item->data(idRole).toInt();
		workouts.remove(id);
		delete item;
	}
}

void WorkoutEditor::addStep()
{
	auto workout = getSelectedWorkout();
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
	auto item = new QListWidgetItem(QIcon(":/icons/next.svg"), step->getCaption(), listWorkoutSteps);
	item->setData(idRole, step->getId());
	listWorkoutSteps->setCurrentItem(item);
}

void WorkoutEditor::deleteStep()
{
	auto item = listWorkoutSteps->takeItem(listWorkoutSteps->currentRow());
	if (item)
	{
		auto id = item->data(idRole).toInt();
		auto workout = getSelectedWorkout();
		if (workout)
		{
			workout->deleteStep(id);
		}
		delete item;
	}
}

void WorkoutEditor::selectedWorkoutChanged()
{
	auto workout = getSelectedWorkout();
	if (workout)
	{
		auto doc = QJsonDocument(workout->getJson().toObject());
		edWorkout->setPlainText(doc.toJson());

		listWorkoutSteps->clear();
		for (auto& step: workout->getSteps())
		{
			auto item = new QListWidgetItem(QIcon(":/icons/next.svg"), step->getCaption(), listWorkoutSteps);
			item->setData(idRole, step->getId());
		}
		listWorkoutSteps->setCurrentRow(0);
	}
}

void WorkoutEditor::selectedWorkoutStepChanged()
{
	loadWorkoutStep(getSelectedWorkoutStep());
}
