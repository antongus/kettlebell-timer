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
const QString sectionSounds    = QStringLiteral("sounds");
const QString varcountdownSoundFileName          = QStringLiteral("countdownSound");
const QString varStartSoundFileName              = QStringLiteral("startSound");
const QString varFinishSoundFileName             = QStringLiteral("finishSound");
const QString varMetronomSoundFileName           = QStringLiteral("metronomSound");
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
	const QJsonObject soundsSettings = conf[sectionSounds].toObject();
	countdownSoundFileName = soundsSettings[varcountdownSoundFileName].toString().trimmed();
	startSoundFileName = soundsSettings[varStartSoundFileName].toString().trimmed();
	finishSoundFileName = soundsSettings[varFinishSoundFileName].toString().trimmed();
	metronomSoundFileName = soundsSettings[varMetronomSoundFileName].toString().trimmed();
}

void Config::toJson(QJsonObject& conf)
{
	QJsonObject soundSettings = conf[sectionSounds].toObject();
	soundSettings[varcountdownSoundFileName] = countdownSoundFileName;
	soundSettings[varStartSoundFileName] = startSoundFileName;
	soundSettings[varFinishSoundFileName] = finishSoundFileName;
	soundSettings[varMetronomSoundFileName] = metronomSoundFileName;
	conf[sectionSounds] = soundSettings;
}
