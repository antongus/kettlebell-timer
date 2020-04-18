/**
*  @file WorkoutPlayer.h
*
*  Workout player class - plays workout program
*
*  Copyright 2020 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

#include <QObject>
#include <QTimer>
#include <memory>

#include "Workout.h"
#include "WorkoutSounds.h"
#include "CountdownTimer.h"

class WorkoutPlayer : public QObject
{
	Q_OBJECT
public:
	explicit WorkoutPlayer(std::shared_ptr<Workout> workout, QObject *parent = nullptr);

signals:
	void displayTicks(QString text);
	void displayAttempts(QString text);
	void done();

public slots:
	void start();
	void stop();

private:
	static constexpr int timerStepMilliseconds { 10 };
	static constexpr bool displayTens { false };

	QTimer* timer;
	WorkoutSounds* sounds;
	std::shared_ptr<Workout> workout { nullptr };
	std::shared_ptr<WorkoutStep> step { nullptr };

	unsigned currentStep { 0 };

	enum class Stage
	{
		Idle,             //!< not started
		InitialPause,     //!< initial pause
		Body,             //!< step body
		BodyPause,        //!< pause between body repeat
		Done,             //!< done
	};

	Stage stage { Stage::Idle };
	qint64 previousTicks;

	CountdownTimer timeCounter;

	int ticksPerAttempt;
	int nextAttemptTicks;
	int attemptCounter;
	int stepAttempts;
	int stepLoops;

	void timerFunction();
	void updateTicks(int ticks);
	void startNextStep();
	void startPreDelay();
	void startBody();
};
