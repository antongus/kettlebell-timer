/**
*  @file Config.h
*
*  Settings structure
*
*  Author : Anton B. Gusev
**/

#pragma once

#include <QString>

class QJsonObject;

struct Config
{
	QString countdownSoundFileName;
	QString startSoundFileName;
	QString finishSoundFileName;
	QString metronomSoundFileName;

	void fromJson(QJsonObject const& conf);
	void toJson(QJsonObject& conf);
};

extern Config config;
