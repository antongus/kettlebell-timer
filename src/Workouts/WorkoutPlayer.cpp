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
	previousTicks = QDateTime::currentMSecsSinceEpoch();
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &WorkoutPlayer::timerFunction);
	timer->start(timerStepMilliseconds);
}

void WorkoutPlayer::start()
{
	stage = Stage::Idle;
	currentStep = 0;
	startNextStep();
}

void WorkoutPlayer::stop()
{
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
		break;

	case Stage::PreCount:
		preCounter -= ticks;
		if (preCounter > 0)
		{
			if (preCounter < nextCountdownTicks)
			{
				nextCountdownTicks -= 1000;
				sounds->playPauseTick();
			}
			updateTicks(preCounter);
		}
		else
		{
			sounds->playStart();
			stage = Stage::Count;
			updateTicks(counter);
		}
		break;

	case Stage::Count:
		counter -= ticks;
		if (counter > 0)
		{
			if (metronomTicks && counter < nextMetronomTicks)
			{
				emit displayAttempts(QString("%1").arg(++attemptsCount));
				nextMetronomTicks -= metronomStep;
				sounds->playAttemptSound();
			}
			updateTicks(counter);
		}
		else
		{
			stage = Stage::Done;
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
	if (displayTens && stage == Stage::Count)
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
	step = workout->getStep(currentStep++);
	if (!step)
	{
		stage = Stage::Done;
		emit done();
		return;
	}
	loopCounter = step->getLoopCount();
	preCounter = step->getInitialDelay() * 1000;
	counter = step->getAttempts();
	if (preCounter)
	{
	    stage = Stage::PreCount;
    }
	else
	{
	    stage = Stage::Count;
    }
}

