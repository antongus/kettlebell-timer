/**
*  @file Config.cpp
*
*  Settings structure
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include "Config.h"

#include <QJsonObject>

Config config;

static constexpr char SectionName_workout[]    = "workout";
static constexpr char SectionName_sounds[]     = "sounds";

static constexpr char VarName_workoutIndex[] = "workoutIndex";
static constexpr char VarName_pauseSound[]   = "pauseSound";
static constexpr char VarName_startSound[]   = "startSound";
static constexpr char VarName_tickSound[]    = "tickSound";
static constexpr char VarName_stopSound[]    = "stopSound";

void Config::fromJson(QJsonObject const& conf)
{
	const QJsonObject soundsSettings = conf[SectionName_sounds].toObject();
	pauseTickSoundFileName = soundsSettings[VarName_pauseSound].toString("bink.wav").trimmed();
	startSoundFileName = soundsSettings[VarName_startSound].toString("start.wav").trimmed();
	finishSoundFileName = soundsSettings[VarName_stopSound].toString("start.wav").trimmed();
	attemptTickSoundFileName = soundsSettings[VarName_tickSound].toString("tick.wav").trimmed();
	const QJsonObject workoutSettings = conf[SectionName_workout].toObject();
	workoutIndex = workoutSettings[VarName_workoutIndex].toInt(0);
}

void Config::toJson(QJsonObject& conf)
{
	QJsonObject soundSettings = conf[SectionName_sounds].toObject();
	soundSettings[VarName_pauseSound] = pauseTickSoundFileName;
	soundSettings[VarName_startSound] = startSoundFileName;
	soundSettings[VarName_stopSound] = finishSoundFileName;
	soundSettings[VarName_tickSound] = attemptTickSoundFileName;
	conf[SectionName_sounds] = soundSettings;

	QJsonObject workoutSettings = conf[SectionName_workout].toObject();
	workoutSettings[VarName_workoutIndex] = workoutIndex;
	conf[SectionName_workout] = workoutSettings;
}
