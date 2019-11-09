/**
*  @file Application.h
*
*  Application class.
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

#include <QApplication>
#include <QString>

class Application : public QApplication
{
	Q_OBJECT
public:
	Application(int& argc, char** argv);
	virtual ~Application() = default;

	static Application* instanse();

	int run();
	static QString const& getSettingsDir();
	static QString const& getLogsDir();
	static QString const& getSoundsDir();
	static QString getConfigFileName(QString const& fileName);
private:
	static QString settingsDirectory;
	static QString logsDirectory;
	static QString soundsDirectory;
	static QtMessageHandler oldHandler;
	static QTranslator translator;
	static void initTranslations();
	static void configureLogger();
	static void configureDirs();
	static void logToFile(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};
