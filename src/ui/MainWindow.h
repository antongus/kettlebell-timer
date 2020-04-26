/**
*  @file MainWindow.h
*
*  Main window
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <memory>

#include "ui_MainWindow.h"
#include "Workout.h"
#include "Workouts.h"
#include "WorkoutPlayer.h"

class MainWindow
        : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

private slots:
	void logMessage(QString const& message);

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	std::unique_ptr<Workouts> workouts { nullptr };
	std::shared_ptr<Workout> workout { nullptr };
	std::shared_ptr<WorkoutPlayer> player { nullptr };

	void loadStyleSheet();
	void connectHandlers();

	void loadConfig();
	void editConfig();
	void saveConfig();

	void loadWorkouts();
	void editWorkouts();
	void saveWorkouts();

	void startStopClicked();
	void stopWorkout();
	void displayTicks(int ticks);

	void selectWorkout();
	void setActiveWorkout(std::shared_ptr<Workout> newWorkout);
};
