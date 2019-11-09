/**
*  @file Application.cpp
*
*  Application class implementation.
*
*  Author : Anton B. Gusev
**/

#include "Application.h"
#include "MainWindow.h"
#include <QApplication>
#include <QTranslator>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QtDebug>

QString Application::settingsDirectory;
QString Application::logsDirectory;
QString Application::soundsDirectory;
QtMessageHandler Application::oldHandler;
QTranslator Application::translator;

Application::Application(int &argc, char **argv)
	: QApplication(argc, argv)
{
	setApplicationName(APP_NAME);
	setApplicationVersion(APP_VERSION);
	setApplicationDisplayName(tr("Kettlebell timer"));
	setOrganizationName(APP_ORGANIZATION);
	configureDirs();
//	configureLogger();
	initTranslations();
}

void Application::logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QDir logsDir(getLogsDir());
	QString logFileName = QString(APP_NAME "-%1.log").arg(QDate::currentDate().toString("yyyy-MM-dd"));
	QString logFilePath = logsDir.absoluteFilePath(logFileName);

	QFile file(logFilePath);
	if(file.open(QFile::WriteOnly | QIODevice::Text | QIODevice::Append))
	{
		QTextStream stream(&file);
		stream << QTime::currentTime().toString("HH:mm:ss:zzz") << " - ";
		switch (type)
		{
		case QtDebugMsg:
			stream << "[DEBUG]: " << msg;
			break;
		case QtInfoMsg:
			stream << "[INFO]: " << msg;
			break;
		case QtWarningMsg:
			stream << "[WARNING]: " << msg;
			break;
		case QtCriticalMsg:
			stream << "[CRITICAL]: " << msg;
			break;
		case QtFatalMsg:
			stream << "[FATAL]: " << msg << "(" << context.file << ", " << context.line << ", " << context.function << ")";
			abort();
		}
		stream << endl;
	}

#ifndef QT_NO_DEBUG
	oldHandler(type, context, msg);
#endif
}

Application* Application::instanse()
{
	return static_cast<Application*>(qApp);
}

void Application::configureLogger()
{
#ifdef QT_NO_DEBUG
#endif
	oldHandler = qInstallMessageHandler(logToFile);
	qInfo() << "------- Starting log -------";
}

/**
 * Configure application directories.
 * Check if application runs in portable configuration (config subdirectory exists
 * in application directory).
 * If so, config and logs are stored in subdirectories of application directory.
 */
void Application::configureDirs()
{
	QDir appDir(applicationDirPath());
	QDir portableSettingsDir(appDir.filePath("config"));
	if (portableSettingsDir.exists())
	{
		settingsDirectory = portableSettingsDir.path();
		logsDirectory = appDir.filePath("logs");
	}
	else
	{
		settingsDirectory = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
		logsDirectory = QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)).filePath("logs");
	}
	soundsDirectory = appDir.filePath("sounds");
}

void Application::initTranslations()
{
	QString locale = QLocale::system().name();
	if (locale != "ru_RU")
	{
		qInfo() << "Loading english translation." << translator.load(":/tr/translation_en.qm");
	}
	else
	{
		qInfo() << "Loading russian translation.";
		translator.load(":/tr/qtbase_ru.qm");
	}
	installTranslator(&translator);
}

int Application::run()
{
	qInfo() << APP_NAME << ", version " << APP_VERSION;

	MainWindow w;
	w.show();

	return exec();
}

QString const& Application::getSettingsDir()
{
	// create settings dir if not exists
	QDir dir(settingsDirectory);
	if (!dir.exists())
		dir.mkpath(".");
	return settingsDirectory;

}

QString const& Application::getLogsDir()
{
	QDir logsDir(logsDirectory);
	if (!logsDir.exists())
		logsDir.mkpath(".");
	return logsDirectory;
}

QString const& Application::getSoundsDir()
{
	QDir soundsDir(soundsDirectory);
	if (!soundsDir.exists())
		soundsDir.mkpath(".");
	return soundsDirectory;
}

QString Application::getConfigFileName(QString const& fileName)
{
	return QString("%1/%2").arg(getSettingsDir()).arg(fileName);
}
