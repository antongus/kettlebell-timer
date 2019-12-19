/**
*  @file MainWindow.cpp
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

#include "MainWindow.h"
#include "AboutDialog.h"
#include "Application.h"
#include "Config.h"
#include "ConfigDialog.h"
#include "WorkoutEditor.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStyle>
#include <QKeyEvent>
#include <QSystemTrayIcon>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , Ui::MainWindow()
{
	setupUi(this);

	// hide settings (not fully implemented yet)
//	tbSettings->setVisible(false);

	loadStyleSheet();
	loadConfig();
	loadWorkouts();

	tickSound = new QSoundEffect(this);
	metronomSound = new QSoundEffect(this);
	startSound = new QSoundEffect(this);
	finishSound = new QSoundEffect(this);
	loadSounds();
	stopExersise();
	connectHandlers();

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MainWindow::timerFunction);
	timer->start(timerStepMilliseconds);
}

MainWindow::~MainWindow()
{
}

void MainWindow::loadStyleSheet()
{
	QFile file(Application::instanse()->getConfigFileName("style.qss"));
	if (file.open(QFile::ReadOnly))
	{
		QString styleSheet = QString::fromLatin1(file.readAll());
		setStyleSheet(styleSheet);
	}
	else
	{
		QFile resFile(":/style/default.qss");
		if (resFile.open(QFile::ReadOnly))
		{
			QString styleSheet = QString::fromLatin1(resFile.readAll());
			setStyleSheet(styleSheet);
		}
	}
}

static constexpr char const configFileName[] { "config.json" };

void MainWindow::loadConfig()
{
	QFile file { Application::instanse()->getConfigFileName(configFileName) };
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray buf = file.readAll();
		auto doc = QJsonDocument::fromJson(buf);
		config.fromJson(doc.object());
	}
}

void MainWindow::saveConfig()
{
	QFile file { Application::instanse()->getConfigFileName(configFileName) };
	if (!file.open(QIODevice::WriteOnly))
	{
		qCritical().noquote() << tr("MainWindow::saveConfig() - error writing to file %1.").arg(file.fileName());
		return;
	}
	QJsonObject jsonObject;
	config.toJson(jsonObject);
	QJsonDocument jsonDocument(jsonObject);
	file.write(jsonDocument.toJson());
}

void MainWindow::editConfig()
{
	ConfigDialog configDialog(this);
	configDialog.SetConfig(config);
	if (configDialog.exec() == QDialog::Accepted)
	{
		configDialog.GetConfig(config);
		saveConfig();
		loadSounds();
	}
}

void MainWindow::editWorkouts()
{
	WorkoutEditor workoutEditor(this);
	auto json = workouts->getJson();
	if (workoutEditor.exec() == QDialog::Accepted)
	{
	}
}

void MainWindow::connectHandlers()
{
	connect(tbAbout, &QToolButton::clicked, [&]() {
		AboutDialog d(this);
		d.exec();
	});

	connect(tbSettings, &QToolButton::clicked, this, &MainWindow::editConfig);
	connect(pbStart, &QPushButton::clicked, this, &MainWindow::startClicked);
	connect(tbEditWorkouts, &QToolButton::clicked, this, &MainWindow::editWorkouts);
}

void MainWindow::startClicked()
{
	pbStart->setEnabled(false);
	startHit = true;
}

void MainWindow::displayTicks(int ticks)
{
	auto seconds = (ticks + 999) / 1000;
	auto minutes = seconds / 60;
	seconds %= 60;
	if (displayTens && stage == Stage::Count)
	{
		auto tens = ((ticks + 99) / 100) % 10;
		auto text = QString("%1:%2.%3").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')).arg(tens);
		lcdTimer->display(text);
	}
	else
	{
		auto text = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
		lcdTimer->display(text);
	}
}

void MainWindow::loadSounds()
{
	QDir dir(Application::instanse()->getSoundsDir());
	tickSound->setSource(QUrl::fromLocalFile(dir.filePath(config.countdownSoundFileName)));
	metronomSound->setSource(QUrl::fromLocalFile(dir.filePath(config.metronomSoundFileName)));
	startSound->setSource(QUrl::fromLocalFile(dir.filePath(config.startSoundFileName)));
	finishSound->setSource(QUrl::fromLocalFile(dir.filePath(config.finishSoundFileName)));
}

void MainWindow::startExersise()
{
	pbStart->setText(tr("Stop! (F2)"));
	pbStart->setToolTip(tr("Stop exersise"));
	pbStart->setIcon(QIcon(":/icons/process-stop.svg"));

	lcdAttempts->display("0");

	preCounter = 10 /* sbCountdown->value() */ * 1000;
	auto minutes = 10; //sbRunTime->value();
	if (!minutes)
		minutes = 10;
	counter = minutes * 60 * 1000;

	attemptsCount = 0;
//	metronomTicks = cbMetronom->isChecked() ? sbMetronom->value() : 0;
	metronomTicks = 200;
	if (metronomTicks)
	{
		metronomStep = counter / metronomTicks;
		nextMetronomTicks = counter - (metronomStep * 2) / 3; // first tick after 2/3 of interval
	}

	if (preCounter)
	{
		stage = Stage::PreCount;
		displayTicks(preCounter);
		tickSound->play();
		nextCountdownTicks = preCounter - 1000;
	}
	else
	{
		stage = Stage::Count;
		displayTicks(counter);
	}
}

void MainWindow::stopExersise()
{
	stage = Stage::Idle;
	pbStart->setText(tr("Start! (F2)"));
	pbStart->setToolTip(tr("Start exersise"));
	pbStart->setIcon(QIcon(":/icons/stopwatch.svg"));
	displayTicks(0);
}

std::optional<Workout> MainWindow::getSelectedWorkout()
{
	return {};
}

static constexpr char const workoutsFileName[] { "workouts.json" };

void MainWindow::loadWorkouts()
{
	if (!workouts)
		workouts = std::make_unique<Workouts>();
	QFile file { Application::instanse()->getConfigFileName(workoutsFileName) };
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray buf = file.readAll();
		auto doc = QJsonDocument::fromJson(buf);
		workouts->setJson(doc.object());
	}
}

void MainWindow::saveWorkouts()
{
	if (!workouts)
		return;
	QFile file { Application::instanse()->getConfigFileName(workoutsFileName) };
	if (!file.open(QIODevice::WriteOnly))
	{
		qCritical().noquote() << tr("MainWindow::saveWorkouts() - error writing to file %1.").arg(file.fileName());
		return;
	}
	QJsonDocument jsonDocument(workouts->getJson().toObject());
	file.write(jsonDocument.toJson());
}

void MainWindow::logMessage(QString const& message)
{
	qDebug() << message;
}

void MainWindow::timerFunction()
{
	auto currentTicks = QDateTime::currentMSecsSinceEpoch();
	auto ticks = currentTicks - previousTicks;
	previousTicks = currentTicks;

	if (startHit)
	{
		if (stage == Stage::Idle)
		{
			startExersise();
		}
		else
		{
			stopExersise();
		}
		startHit = false;
		pbStart->setEnabled(true);
		pbStart->setShortcut(QKeySequence("F2"));
		return;
	}

	switch (stage)
	{
	case Stage::Idle:
		break;

	case Stage::PreCount:
		preCounter -= ticks;
		if (preCounter > 0)
		{
			if (preCounter < nextCountdownTicks)
			{
				nextCountdownTicks -= 1000;
				tickSound->play();
			}
			displayTicks(preCounter);
		}
		else
		{
			startSound->play();
			stage = Stage::Count;
			displayTicks(counter);
		}
		break;

	case Stage::Count:
		counter -= ticks;
		if (counter > 0)
		{
			if (metronomTicks && counter < nextMetronomTicks)
			{
				lcdAttempts->display(QString("%1").arg(++attemptsCount));
				nextMetronomTicks -= metronomStep;
				metronomSound->play();
			}
			displayTicks(counter);
		}
		else
		{
			stage = Stage::Done;
		}
		break;

	case Stage::Done:
		startSound->play();
		displayTicks(0);
		stage = Stage::Idle;
		break;
	}
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Escape && isFullScreen())
	{
		setWindowState(Qt::WindowNoState);
		event->accept();
	}
	else
	{
		auto key = event->key();
		if (false
		    || key == Qt::Key_F11
		    || ((key == Qt::Key_Return || key == Qt::Key_Enter) && event->modifiers() & Qt::AltModifier)
		    )
		{
			setWindowState(isFullScreen() ? Qt::WindowNoState : Qt::WindowFullScreen);
			event->accept();
		}
		else
		{
			QMainWindow::keyPressEvent(event);
		}
	}
}

