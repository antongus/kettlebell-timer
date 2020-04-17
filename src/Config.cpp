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

namespace
{
struct SectionNames
{
	static constexpr char workout[]    = "workout";
	static constexpr char sounds[]     = "sounds";
};

struct VarNames
{
	static constexpr char workoutIndex[] = "workoutIndex";
	static constexpr char pauseSound[]   = "pauseSound";
	static constexpr char startSound[]   = "startSound";
	static constexpr char tickSound[]    = "tickSound";
	static constexpr char stopSound[]    = "stopSound";
};

}

void Config::loadDefaults()
{
	countdownSoundFileName = "bink.wav";
	startSoundFileName = "start.wav";
	finishSoundFileName = "start.wav";
	metronomSoundFileName = "tick.wav";
}

void Config::fromJson(QJsonObject const& conf)
{
	const QJsonObject soundsSettings = conf[SectionNames::sounds].toObject();
	countdownSoundFileName = soundsSettings[VarNames::pauseSound].toString().trimmed();
	startSoundFileName = soundsSettings[VarNames::startSound].toString().trimmed();
	finishSoundFileName = soundsSettings[VarNames::stopSound].toString().trimmed();
	metronomSoundFileName = soundsSettings[VarNames::tickSound].toString().trimmed();
	const QJsonObject workoutSettings = conf[SectionNames::workout].toObject();
	workoutIndex = workoutSettings[VarNames::workoutIndex].toInt(0);
}

void Config::toJson(QJsonObject& conf)
{
	QJsonObject soundSettings = conf[SectionNames::sounds].toObject();
	soundSettings[VarNames::pauseSound] = countdownSoundFileName;
	soundSettings[VarNames::startSound] = startSoundFileName;
	soundSettings[VarNames::stopSound] = finishSoundFileName;
	soundSettings[VarNames::tickSound] = metronomSoundFileName;
	conf[SectionNames::sounds] = soundSettings;

	QJsonObject workoutSettings = conf[SectionNames::workout].toObject();
	workoutSettings[VarNames::workoutIndex] = workoutIndex;
	conf[SectionNames::workout] = workoutSettings;
}
