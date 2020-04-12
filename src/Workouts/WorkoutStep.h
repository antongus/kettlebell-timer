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

#include "ConfigItem.h"

class WorkoutStep
        : public QObject
        , public ConfigItem
{
	Q_OBJECT
public:
	WorkoutStep();
	virtual ~WorkoutStep() override = default;

	QJsonValue getJson() const override;
	void setJson(const QJsonValue& conf) override;

	int getId() const { return id; }
	QString getCaption() const { return caption; }
	qint64 getDelayBeforeStart() const { return delayBeforeStart; }
	qint64 getDuration() const { return duration; }
	int getRepeatCount() const { return repeatCount; }
	qint64 getPauseBetweenRepeats() const { return pauseBetweenRepeats; }

	void setCaption(QString value) { caption = value; }

protected:
	static constexpr qint64 second { 1000 };
	static constexpr qint64 minute { 60 * second };

	int id;

	QString caption;                           //!< step title
	qint64 delayBeforeStart { 10 * second };   //!< delay before start, ms
	qint64 duration { 20 * minute };           //!< step duration, ms
	int repeatCount { 1 };                     //!< step repeats count
	qint64 pauseBetweenRepeats { 5 * second }; //!< pause between repeats (if repeat count more than 1), ms

};
