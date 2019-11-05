/**
*  @file MainWindow.cpp
*
*  Main window
*
*  Copyright © 2019 Anton B. Gusev
**/

#include "MainWindow.h"
#include "AboutDialog.h"
#include "Application.h"
#include "Config.h"
#include "ConfigDialog.h"

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
	tbSettings->setVisible(false);

	loadStyleSheet();
	loadConfig();

	tickSound = new QSoundEffect(this);
	metronomSound = new QSoundEffect(this);
	startSound = new QSoundEffect(this);
	loadSounds();

	displayTicks(0);

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
		resFile.open(QFile::ReadOnly);
		QString styleSheet = QString::fromLatin1(resFile.readAll());
		setStyleSheet(styleSheet);
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
		qCritical().noquote() << tr("MainWindow::saveConfig() - ошибка записи в файл %1.").arg(file.fileName());
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


/// подключить обработчики кнопок и меню
void MainWindow::connectHandlers()
{
	connect(tbAbout, &QToolButton::clicked, [&]() {
		AboutDialog d(this);
		d.exec();
	});

	connect(tbSettings, &QToolButton::clicked, this, &MainWindow::editConfig);
	connect(pbStart, &QPushButton::clicked, this, &MainWindow::startClicked);
	connect(cbMetronom, &QCheckBox::stateChanged, [&](){ sbMetronom->setEnabled(cbMetronom->isChecked()); });
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
	tickSound->setSource(QUrl::fromLocalFile(dir.filePath("bink.wav")));
	metronomSound->setSource(QUrl::fromLocalFile(dir.filePath("tick.wav")));
	startSound->setSource(QUrl::fromLocalFile(dir.filePath("start.wav")));
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
			pbStart->setText(tr("Стоп! (F2)"));
			pbStart->setToolTip(tr("Остановить счёт"));
			pbStart->setIcon(QIcon(":/icons/process-stop.svg"));

			preCounter = sbCountdown->value() * 1000;
			auto minutes = cbTryMinutes->currentText().toInt();
			if (!minutes)
				minutes = 10;
			counter = minutes * 60 * 1000;

			attemptsCount = 0;
			metronomTicks = cbMetronom->isChecked() ? sbMetronom->value() : 0;
			if (metronomTicks)
			{
				metronomStep = counter / metronomTicks;
				nextMetronomTicks = counter - metronomStep/2; // first tick in the middle of interval
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
		else
		{
			stage = Stage::Idle;
			pbStart->setText(tr("Старт! (F2)"));
			pbStart->setToolTip(tr("Начать счёт"));
			pbStart->setIcon(QIcon(":/icons/stopwatch.svg"));
			displayTicks(0);
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
