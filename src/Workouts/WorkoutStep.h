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
	int  getInitialDelay() const { return initialDelay; }
	int  getDuration() const { return duration; }
	int  getAttempts() const { return attempts; }
	int  getLoopCount() const { return loopCount; }
	int  getLoopPause() const { return loopPause; }

	void setCaption(QString value) { caption = value; }
	void setInitialDelay(int value) { initialDelay = value; }
	void setDuration(int value) { duration = value; }
	void setAttempts(int value) { attempts = value; }
	void setLoopCount(int value) { loopCount = value; }
	void setLoopPause(int value) { loopPause = value; }

protected:
	static constexpr int second { 1 };
	static constexpr int minute { 60 * second };

	QString caption;                          //!< step title
	int  initialDelay { 10 * second };        //!< delay before start (seconds)
	int  duration { 10 * minute };            //!< step duration (seconds)
	int  attempts { 200 };                    //!< attempts during step
	int  loopCount { 1 };                     //!< how many times should repeat step
	int  loopPause { 5 * second };            //!< pause before second and all next repeats in loop

};
