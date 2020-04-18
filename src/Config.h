/**
*  @file Config.h
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

#pragma once

#include <QString>

class QJsonObject;

struct Config
{
	int workoutIndex;
	QString pauseTickSoundFileName;
	QString startSoundFileName;
	QString finishSoundFileName;
	QString attemptTickSoundFileName;

	void loadDefaults();
	void fromJson(QJsonObject const& conf);
	void toJson(QJsonObject& conf);
};

extern Config config;
