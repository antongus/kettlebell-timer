/**
*  @file Config.cpp
*
*  Settings structure
*
*  Author : Anton B. Gusev
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
