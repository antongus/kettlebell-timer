/**
*  @file WorkoutStep.h
*
*  WorkoutStep class - single step of workout
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <memory>

#include "ItemWithId.h"
#include "ConfigItem.h"

class WorkoutStep
        : public ItemWithId
        , public ConfigItem
{
public:
	WorkoutStep() = default;
	virtual ~WorkoutStep() override = default;

	QJsonValue getJson() const override;
	void setJson(const QJsonValue& conf) override;

	QString getCaption() const { return caption; }
	int getDelayBeforeStart() const { return delayBeforeStart; }
	int getDuration() const { return duration; }
	int getRepeatCount() const { return repeatCount; }
	int getPauseBetweenRepeats() const { return pauseBetweenRepeats; }

	void setCaption(QString value) { caption = value; }
	void setDelayBeforeStart(int value) { delayBeforeStart = value; }
	void setDuration(int value) { duration = value; }
	void setRepeatCount(int value) { repeatCount = value; }
	void setPauseBetweenRepeats(int value) { pauseBetweenRepeats = value; }

protected:
	static constexpr int second { 1 };
	static constexpr int minute { 60 * second };

	QString caption;                           //!< step title
	int delayBeforeStart { 10 * second };   //!< delay before start, ms
	int duration { 20 * minute };           //!< step duration, ms
	int repeatCount { 1 };                     //!< step repeats count
	int pauseBetweenRepeats { 5 * second }; //!< pause between repeats (if repeat count more than 1), ms

};
