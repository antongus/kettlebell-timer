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
	int  getStartDelay() const { return startDelay; }
	int  getRoundTime() const { return roundTime; }
	int  getRoundAttempts() const { return roundAttempts; }
	int  getRoundCount() const { return roundCount; }
	int  getRestTime() const { return restTime; }

	void setCaption(QString value) { caption = value; }
	void setStartDelay(int value) { startDelay = value; }
	void setRoundTime(int value) { roundTime = value; }
	void setRoundAttempts(int value) { roundAttempts = value; }
	void setRoundCount(int value) { roundCount = value; }
	void setRestTime(int value) { restTime = value; }

protected:
	static constexpr int second { 1 };
	static constexpr int minute { 60 * second };

	QString caption;                        //!< step title
	int  startDelay { 10 * second };        //!< delay before start (seconds)
	int  roundTime { 10 * minute };         //!< round duration (seconds)
	int  roundAttempts { 200 };             //!< attempts in round
	int  roundCount { 1 };                  //!< how many rounds in step
	int  restTime { 5 * second };           //!< rest between rounds

};
