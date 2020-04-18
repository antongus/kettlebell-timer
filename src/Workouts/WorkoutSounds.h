/**
*  @file WorkoutSounds.h
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

#pragma once

#include <QObject>

class QSoundEffect;

class WorkoutSounds : public QObject
{
	Q_OBJECT
public:
	explicit WorkoutSounds(QObject *parent = nullptr);

public slots:
	void playPauseTick();
	void playStart();
	void playAttemptSound();
	void playFinish();

private:
	QSoundEffect* pauseTickSound { nullptr };
	QSoundEffect* attemptSound { nullptr };
	QSoundEffect* startSound { nullptr };
	QSoundEffect* finishSound { nullptr };

	void stop();
};
