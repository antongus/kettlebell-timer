/**
*  @file WorkoutEditor.h
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

#pragma once

#include <QDialog>
#include <QAction>

#include "ui_WorkoutEditor.h"
#include "Workouts.h"

class WorkoutEditor
        : public QDialog
        , private Ui::WorkoutEditor
{
	Q_OBJECT

public:
	explicit WorkoutEditor(QWidget *parent = nullptr);
	~WorkoutEditor();

	QJsonValue getJson();
	void setJson(const QJsonValue& conf);

private:
	static constexpr int idRole {  Qt::UserRole };
	Workouts workouts;

	std::shared_ptr<Workout> getSelectedWorkout();
	std::shared_ptr<WorkoutStep> getSelectedWorkoutStep();
	void loadWorkoutStep(std::shared_ptr<WorkoutStep> step);

	std::shared_ptr<WorkoutStep> currentWorkoutStep { nullptr };

private slots:

	void addWorkout();
	void deleteWorkout();
	void addStep();
	void deleteStep();
	void moveStepUp();
	void moveStepDown();
	void saveCurrentWorkoutStep();
	void selectedWorkoutChanged();
	void selectedWorkoutStepChanged();

};
