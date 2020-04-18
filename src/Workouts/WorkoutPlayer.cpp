/**
*  @file WorkoutPlayer.cpp
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

#include "WorkoutPlayer.h"

#include <QDateTime>

WorkoutPlayer::WorkoutPlayer(std::shared_ptr<Workout> workout, QObject *parent)
    : QObject(parent)
    , workout(workout)
{
	sounds = new WorkoutSounds(this);
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &WorkoutPlayer::timerFunction);
}

void WorkoutPlayer::start()
{
	stage = Stage::Idle;
	currentStep = 0;
	timer->start(timerStepMilliseconds);
}

void WorkoutPlayer::stop()
{
	timer->stop();
	stage = Stage::Done;
}

void WorkoutPlayer::timerFunction()
{
	auto currentTicks = QDateTime::currentMSecsSinceEpoch();
	auto ticks = currentTicks - previousTicks;
	previousTicks = currentTicks;

	switch (stage)
	{
	case Stage::Idle:
		startNextStep();
		break;

	case Stage::InitialPause:
		if (timeCounter.update(ticks))
			sounds->playPauseTick();
		updateTicks(timeCounter.getTicks());
		if (timeCounter.isElapsed())
		{
			sounds->playStart();
			startBody();
		}
		break;

	case Stage::Body:
		timeCounter.update(ticks);
		updateTicks(timeCounter.getTicks());
		if (stepAttempts) // need to count attempts
		{
			if (timeCounter.getTicks() < nextAttemptTicks)
			{
				emit displayAttempts(QString("%1").arg(++attemptCounter));
				nextAttemptTicks -= ticksPerAttempt;
				sounds->playAttemptSound();
			}
			updateTicks(timeCounter.getTicks());
		}
		if (timeCounter.isElapsed())
		{
			sounds->playFinish();
			startBody();
		}
		break;

	case Stage::BodyPause:
		if (timeCounter.update(ticks))
			sounds->playPauseTick();
		updateTicks(timeCounter.getTicks());
		if (timeCounter.isElapsed())
		{
			startBody();
		}
		break;

	case Stage::Done:
		sounds->playFinish();
		updateTicks(0);
		stage = Stage::Idle;
		break;
	}
}

void WorkoutPlayer::updateTicks(int ticks)
{
	auto seconds = (ticks + 999) / 1000;
	auto minutes = seconds / 60;
	seconds %= 60;
	if (displayTens && stage == Stage::Body)
	{
		auto tens = ((ticks + 99) / 100) % 10;
		auto text = QString("%1:%2.%3").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')).arg(tens);
		emit displayTicks(text);
	}
	else
	{
		auto text = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
		emit displayTicks(text);
	}
}

void WorkoutPlayer::startNextStep()
{
	// try to get next step...
	step = workout->getStep(currentStep++);

	if (!step)  // no next step - done
	{
		stop();
		emit done();
		return;
	}

	// load counters for step
	stepLoops = step->getLoopCount();

	if (step->getInitialDelay())
		startPreDelay();
	else
		startBody();
}

/// start pre-delay stage
void WorkoutPlayer::startPreDelay()
{
	// start countdown timer for initial delay
	timeCounter.start(step->getInitialDelay() * 1000);

	// switch stage
	stage = Stage::InitialPause;

	// zero-out attempt counter
	emit displayAttempts("0");
}

void WorkoutPlayer::startBody()
{
	if (stepLoops--) // step has loops to play
	{
		// start countdown timer for step
		auto duration = step->getDuration() * 1000;
		timeCounter.start(duration);

		// prepare counters for attempts counter
		attemptCounter = 0;
		stepAttempts = step->getAttempts();
		if (stepAttempts)
		{
			ticksPerAttempt = duration / stepAttempts;
			nextAttemptTicks = duration - (ticksPerAttempt * 3) / 4; // first tick after 3/4 of interval
		}
		emit displayAttempts("0");
		stage = Stage::Body;
		sounds->playStart();
	}
	else
		startNextStep();
}

