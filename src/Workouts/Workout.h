/**
*  @file Workout.h
*
*  Workout class - set or workout steps
*
*  Copyright 2020 Anton B. Gusev
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
#include <vector>
#include <memory>

#include "WorkoutStep.h"
#include "ItemWithId.h"
#include "ConfigItem.h"

class Workout
        : public QObject
        , public ItemWithId
        , public ConfigItem
{
	Q_OBJECT
public:
	Workout();
	~Workout() override;

	void setTitle(QString value) { title = std::move(value); }
	QString const& getTitle() const { return title; }

	// ConfigItem interface
	QJsonValue getJson() const override;
	void setJson(const QJsonValue& conf) override;

	std::vector<std::shared_ptr<WorkoutStep>>& getSteps() { return steps; }
	std::shared_ptr<WorkoutStep> getStep(unsigned index);
	std::shared_ptr<WorkoutStep> findStep(int id);
	std::shared_ptr<WorkoutStep> addStep(QString const& title);
	void swapSteps(int index1, int index2);
	void deleteStep(int id);

public slots:
	bool start();
	void stop();
	void timer();

signals:
	void started();
	void finished();
	void stageChanged(QString const& stageText);
	void timeChanged(int seconds);
	void attemptsChanged(int attempts);
	void playSound();

private:
	QString title;
	std::vector<std::shared_ptr<WorkoutStep>> steps;
	bool running { false };
	std::size_t currentStep { 0 };
};
