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
#include "WorkoutSelector.h"

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

static constexpr char const configFileName[] { "config.json" };
static constexpr char const workoutsFileName[] { "workouts.json" };

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , Ui::MainWindow()
{
	setupUi(this);

	loadStyleSheet();
	loadConfig();

	workouts = std::make_unique<Workouts>();
	loadWorkouts();

	setActiveWorkout(workouts->getWorkout(static_cast<unsigned>(config.workoutIndex)));

	connectHandlers();
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

void MainWindow::loadConfig()
{
	QFile file { Application::instanse()->getConfigFileName(configFileName) };
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray buf = file.readAll();
		auto doc = QJsonDocument::fromJson(buf);
		config.fromJson(doc.object());
	}
	else
	{
		// create and default config
		config.fromJson(QJsonObject());
		saveConfig();
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
	}
}

void MainWindow::editWorkouts()
{
	WorkoutEditor workoutEditor(this);
	workoutEditor.setJson(workouts->getJson());
	if (workoutEditor.exec() == QDialog::Accepted)
	{
		workouts->setJson(workoutEditor.getJson());
		saveWorkouts();
		setActiveWorkout(workouts->getWorkout(static_cast<unsigned>(config.workoutIndex)));
	}
}

void MainWindow::selectWorkout()
{
	WorkoutSelector workoutSelector(workouts.get(), config.workoutIndex, this);
	if (workoutSelector.exec() == QDialog::Accepted)
	{
		auto index = workoutSelector.getSelectedWorkoutIndex();
		if (index != config.workoutIndex)
		{
			config.workoutIndex = index;
			saveConfig();
		}
		auto newWorkout = workouts->getWorkout(static_cast<unsigned>(index));
		setActiveWorkout(newWorkout);
	}
}

void MainWindow::connectHandlers()
{
	connect(tbAbout, &QToolButton::clicked, [&]() {
		AboutDialog d(this);
		d.exec();
	});

	connect(tbSettings, &QToolButton::clicked, this, &MainWindow::editConfig);
	connect(pbStart, &QPushButton::clicked, this, &MainWindow::startStopClicked);
	connect(tbEditWorkouts, &QToolButton::clicked, this, &MainWindow::editWorkouts);
	connect(tbSelectWorkout, &QToolButton::clicked, this, &MainWindow::selectWorkout);
}

void MainWindow::startStopClicked()
{
	pbStart->setEnabled(false);
	if (player)
	{
		stopWorkout();
	}
	else if (workout)
	{
		player = std::make_shared<WorkoutPlayer>(workout);
		auto p = player.get();
		connect(p, &WorkoutPlayer::displayTicks, [&](QString text){ lcdTimer->display(text); });
		connect(p, &WorkoutPlayer::displayAttempts, [&](QString text){ lcdAttempts->display(text); });
		connect(p, &WorkoutPlayer::displayStep, [&](QString text){ labelStepCaption->setText(text); });
		connect(p, &WorkoutPlayer::displayStage, [&](QString text){ labelStageCaption->setText(text); });
		connect(p, &WorkoutPlayer::done, this, &MainWindow::stopWorkout);

		pbStart->setText(tr("Stop! (F2)"));
		pbStart->setToolTip(tr("Stop exersise"));
		pbStart->setIcon(QIcon(":/icons/process-stop.svg"));

		player->start();
	}
	pbStart->setEnabled(true);
	pbStart->setShortcut(QKeySequence("F2"));
}

void MainWindow::stopWorkout()
{
	if (player)
	{
		player->stop();
		player = nullptr;

		pbStart->setText(tr("Start! (F2)"));
		pbStart->setToolTip(tr("Start exersise"));
		pbStart->setIcon(QIcon(":/icons/stopwatch.svg"));
		pbStart->setShortcut(QKeySequence("F2"));
	}
}


void MainWindow::loadWorkouts()
{
	QFile file { Application::instanse()->getConfigFileName(workoutsFileName) };
	if (file.open(QIODevice::ReadOnly))
	{
		QByteArray buf = file.readAll();
		auto doc = QJsonDocument::fromJson(buf);
		workouts->setJson(doc.object());
	}
	else
	{
		workouts->createDefaultWorkouts();
		saveWorkouts();
	}
}

void MainWindow::saveWorkouts()
{
	QFile file { Application::instanse()->getConfigFileName(workoutsFileName) };
	if (!file.open(QIODevice::WriteOnly))
	{
		qCritical().noquote() << tr("MainWindow::saveWorkouts() - error writing to file %1.").arg(file.fileName());
		return;
	}
	QJsonDocument jsonDocument(workouts->getJson().toObject());
	file.write(jsonDocument.toJson());
}

void MainWindow::setActiveWorkout(std::shared_ptr<Workout> newWorkout)
{
	stopWorkout();
	workout = newWorkout;
	if (workout)
	{
		labelWorkoutCaption->setText(workout->getTitle());
	}
	else
	{
		labelWorkoutCaption->setText(tr("NO WORKOUT SELECTED"));
	}
}

void MainWindow::logMessage(QString const& message)
{
	qDebug() << message;
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

