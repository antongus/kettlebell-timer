/**
*  @file MainWindow.h
*
*  Main window
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once
#include "ui_MainWindow.h"
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QSoundEffect>


class MainWindow
        : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

private slots:
	void logMessage(QString const& message);
	void timerFunction();

protected:
	void keyPressEvent(QKeyEvent* event) override;

private:
	static constexpr int timerStepMilliseconds { 10 };
	static constexpr bool displayTens { false };

	QTimer* timer { nullptr };

	QSoundEffect* tickSound { nullptr };
	QSoundEffect* metronomSound { nullptr };
	QSoundEffect* startSound { nullptr };

	enum class Stage
	{
		Idle,
		PreCount,
		Count,
		Done,
	};

	Stage stage { Stage::Idle };
	qint64 previousTicks;
	bool startHit { false };
	int preCounter;
	int counter;
	int metronomTicks;
	int attemptsCount;
	int metronomStep;
	int nextCountdownTicks;
	int nextMetronomTicks;

	void loadStyleSheet();
	void loadConfig();
	void saveConfig();
	void editConfig();
	void connectHandlers();
	void startClicked();
	void displayTicks(int ticks);
	void loadSounds();
};
