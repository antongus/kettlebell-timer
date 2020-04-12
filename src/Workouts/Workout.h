/**
*  @file Workout.h
*
*  Workout class - set or workout steps
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
#include <vector>
#include <optional>
#include <memory>

#include "WorkoutStep.h"
#include "ConfigItem.h"

class Workout
        : public QObject
        , public ConfigItem
{
	Q_OBJECT
public:
	Workout();
	~Workout() override;

	Workout(Workout const& other) = delete;
	Workout(Workout&& other) = delete;
	Workout& operator= (Workout const& other) = delete;
	Workout& operator= (Workout&& other) = delete;

	void setTitle(QString value) { title = std::move(value); }
	QString const& getTitle() const { return title; }

	int getId() const { return id; }

	// ConfigItem interface
	QJsonValue getJson() const override;
	void setJson(const QJsonValue& conf) override;

	std::shared_ptr<WorkoutStep> findStep(QString const& title);
	std::shared_ptr<WorkoutStep> addStep(QString const& title);
	bool deleteStep(std::shared_ptr<WorkoutStep> step);

	auto begin() { return std::begin(steps); }
	auto end() { return std::end(steps); }
	auto begin() const { return std::begin(steps); }
	auto end() const { return std::end(steps); }

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
	int id;
	QString title;
	std::vector<std::shared_ptr<WorkoutStep>> steps;
	bool running { false };
	std::size_t currentStep { 0 };
};
