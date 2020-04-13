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
#include <QtDebug>

#include "WorkoutEditor.h"

WorkoutEditor::WorkoutEditor(QWidget *parent)
    : QDialog(parent)
{
	setupUi(this);

	// Ok and Cancel buttons
	connect(pbOk, &QPushButton::clicked, [=]{done(QDialog::Accepted); });
	connect(pbCancel, &QPushButton::clicked, [=]{done(QDialog::Rejected); });

	// create actions
	auto actionAddWorkout = new QAction(QIcon(":/icons/add_workout.svg"), tr("Add new workout"), this);
	connect(actionAddWorkout, &QAction::triggered, this, &WorkoutEditor::addWorkout);

	auto actionDeleteWorkout = new QAction(QIcon(":/icons/delete_workout.svg"), tr("Delete selected workout"), this);
	connect(actionDeleteWorkout, &QAction::triggered, this, &WorkoutEditor::deleteWorkout);

	auto actionAddStepWork = new QAction(QIcon(":/icons/hurdle.svg"), tr("Add step to workout"), this);
	connect(actionAddStepWork, &QAction::triggered, this, &WorkoutEditor::addStep);

	auto actionDeleteStep = new QAction(QIcon(":/icons/delete_step.svg"), tr("Delete step from workout"), this);
	connect(actionDeleteStep, &QAction::triggered, this, &WorkoutEditor::deleteStep);

	auto actionMoveStepUp = new QAction(QIcon(":/icons/up.svg"), tr("Move step up"), this);
	connect(actionMoveStepUp, &QAction::triggered, this, &WorkoutEditor::moveStepUp);

	auto actionMoveStepDown = new QAction(QIcon(":/icons/down.svg"), tr("Move step down"), this);
	connect(actionMoveStepDown, &QAction::triggered, this, &WorkoutEditor::moveStepDown);

	// assign actions to buttons
	tbAddWorkout->setDefaultAction(actionAddWorkout);
	tbDeleteWorkout->setDefaultAction(actionDeleteWorkout);
	tbAddStepWork->setDefaultAction(actionAddStepWork);
	tbDeleteStep->setDefaultAction(actionDeleteStep);
	tbMoveStepUp->setDefaultAction(actionMoveStepUp);
	tbMoveStepDown->setDefaultAction(actionMoveStepDown);

	loadWorkoutStep(nullptr);

	connect(listWorkoutSteps, &QListWidget::currentItemChanged, this, &WorkoutEditor::selectedWorkoutStepChanged);

	listWorkouts->setModel(&workouts);
	connect(listWorkouts->selectionModel(), &QItemSelectionModel::selectionChanged, this, &WorkoutEditor::selectedWorkoutChanged);
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
	listWorkouts->setCurrentIndex(workouts.index(0));
}

/**
 * get current (selected in left pane) workout
 */
std::shared_ptr<Workout> WorkoutEditor::getSelectedWorkout()
{
	auto index = listWorkouts->currentIndex();
	if (index.isValid())
	{
		auto row = static_cast<size_t>(index.row());
		return workouts.getWorkout(row);
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
		auto caption = edStepCaption->text();
		currentWorkoutStep->setCaption(caption);
		currentWorkoutStep->setDelayBeforeStart(sbDelayBeforeStart->value());
		currentWorkoutStep->setDuration(sbStepDuration->value());
		currentWorkoutStep->setRepeatCount(sbStepRepeatCount->value());
		currentWorkoutStep->setPauseBetweenRepeats(sbPauseBetweenRepeats->value());
		auto id = currentWorkoutStep->getId();
		for (auto row = 0; row < listWorkoutSteps->count(); ++row)
		{
			auto item = listWorkoutSteps->item(row);
			auto itemId = item->data(idRole).toInt();
			if (id == itemId)
			{
				item->setText(caption);
				break;
			}
		}
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
}

void WorkoutEditor::deleteWorkout()
{
	auto index = listWorkouts->currentIndex();
	if (index.isValid())
	{
		auto row = index.row();
		auto workout = workouts.getWorkout(static_cast<size_t>(row));
		if (workout)
		{
			workouts.remove(workout->getId());
			if (row >= static_cast<int>(workouts.size()))
				row--;
			listWorkouts->setCurrentIndex(workouts.index(row));
		}
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

void WorkoutEditor::moveStepUp()
{
	auto row = listWorkoutSteps->currentRow();
	if (row > 0)
	{
		auto item = listWorkoutSteps->takeItem(row);
		auto newRow = row - 1;
		listWorkoutSteps->insertItem(newRow, item);
		listWorkoutSteps->setCurrentRow(newRow);
	}
}

void WorkoutEditor::moveStepDown()
{
	auto row = listWorkoutSteps->currentRow();
	auto cnt = listWorkoutSteps->count();
	if (cnt > 1 && row < cnt - 1)
	{
		auto item = listWorkoutSteps->takeItem(row);
		auto newRow = row + 1;
		listWorkoutSteps->insertItem(newRow, item);
		listWorkoutSteps->setCurrentRow(newRow);
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
