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

#include "ui_WorkoutSelector.h"
#include "Workouts.h"

class WorkoutSelector
        : public QDialog
        , private Ui:: WorkoutSelector
{
	Q_OBJECT

public:
	explicit WorkoutSelector(Workouts* workouts, int currentIndex, QWidget *parent = nullptr);
	~WorkoutSelector();

	int getSelectedWorkoutIndex();

private:
	Workouts* workouts;

	void listDoubleClicked(const QModelIndex& index);
};
