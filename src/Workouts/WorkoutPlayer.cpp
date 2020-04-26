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
	emit displayStep(tr("---"));
	emit displayStage(tr("---"));
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
			startRound();
		}
		break;

	case Stage::Leg:
		timeCounter.update(ticks);
		updateTicks(timeCounter.getTicks());
		if (stepAttempts) // need to count attempts
		{
			if (timeCounter.getTicks() <= nextAttemptTicks)
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
			if (remainingRounds) // need to repeat the body?
			{
				timeCounter.start(step->getRestTime() * 1000);
				stage = Stage::LegPause;
				auto rounds = step->getRoundCount();
				if (rounds > 1)
					emit displayStage(tr("REST (ROUND %1 OF %2)")
				                      .arg(rounds - remainingRounds)
				                      .arg(rounds)
				                      );
				else
					emit displayStage(tr("REST"));
			}
			else
				startNextStep();
		}
		break;

	case Stage::LegPause:
		if (timeCounter.update(ticks))
			sounds->playPauseTick();
		updateTicks(timeCounter.getTicks());
		if (timeCounter.isElapsed())
		{
			startRound();
		}
		break;

	case Stage::Finishing:
		timeCounter.update(ticks);
		if (timeCounter.isElapsed())
		{
			stop();
			emit done();
		}
		break;

	case Stage::Done:
		sounds->playFinish();
		updateTicks(0);
		break;
	}
}

void WorkoutPlayer::updateTicks(int ticks)
{
	auto seconds = (ticks + 999) / 1000;
	auto minutes = seconds / 60;
	seconds %= 60;
	if (displayTens && stage == Stage::Leg)
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

	if (!step)  // no next step - play finish sound
	{
		sounds->playFinish();
		// start countdown timer for finish delay
		timeCounter.start(2 * 1000);

		// switch stage
		stage = Stage::Finishing;
		return;
	}

	emit displayStep(step->getCaption());

	remainingRounds = step->getRoundCount();

	if (step->getStartDelay())
		startPreDelay();
	else
		startRound();
}

/// start pre-delay stage
void WorkoutPlayer::startPreDelay()
{
	// start countdown timer for initial delay
	timeCounter.start(step->getStartDelay() * 1000);

	// switch stage
	stage = Stage::InitialPause;

	// zero-out attempt counter
	emit displayAttempts("0");

	// display stage
	emit displayStage(tr("DELAY BEFORE STEP START"));
}

void WorkoutPlayer::startRound()
{
	if (remainingRounds--) // current step has a legs to play
	{
		// start countdown timer for step
		auto duration = step->getRoundTime() * 1000;
		timeCounter.start(duration);

		// prepare counters for attempts counter
		attemptCounter = 0;
		stepAttempts = step->getRoundAttempts();
		if (stepAttempts)
		{
			ticksPerAttempt = duration / stepAttempts;
			nextAttemptTicks = duration - ticksPerAttempt;
		}
		emit displayAttempts("0");
		stage = Stage::Leg;
		auto rounds = step->getRoundCount();
		if (rounds > 1)
			emit displayStage(tr("WORK (ROUND %1 OF %2)")
		                      .arg(rounds - remainingRounds)
		                      .arg(rounds)
		                      );
		else
			emit displayStage(tr("WORK"));
		sounds->playStart();
	}
	else
		startNextStep();
}
