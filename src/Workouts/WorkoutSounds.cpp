/**
*  @file WorkoutSounds.cpp
*
*  Workout sounds
*
*  Copyright 2020 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include "WorkoutSounds.h"
#include "Config.h"
#include "Application.h"

#include <QSoundEffect>
#include <QDir>

WorkoutSounds::WorkoutSounds(QObject *parent) : QObject(parent)
{
	pauseTickSound = new QSoundEffect(this);
	attemptSound = new QSoundEffect(this);
	startSound = new QSoundEffect(this);
	finishSound = new QSoundEffect(this);

	QDir dir(Application::instanse()->getSoundsDir());
	pauseTickSound->setSource(QUrl::fromLocalFile(dir.filePath(config.pauseTickSoundFileName)));
	attemptSound->setSource(QUrl::fromLocalFile(dir.filePath(config.attemptTickSoundFileName)));
	startSound->setSource(QUrl::fromLocalFile(dir.filePath(config.startSoundFileName)));
	finishSound->setSource(QUrl::fromLocalFile(dir.filePath(config.finishSoundFileName)));
}

void WorkoutSounds::playPauseTick()
{
	stop();
	pauseTickSound->play();
}

void WorkoutSounds::playStart()
{
	stop();
	startSound->play();
}

void WorkoutSounds::playAttemptSound()
{
	stop();
	attemptSound->play();
}

void WorkoutSounds::playFinish()
{
	stop();
	finishSound->play();
}

void WorkoutSounds::stop()
{
	pauseTickSound->stop();
	attemptSound->stop();
	startSound->stop();
	finishSound->stop();
}
