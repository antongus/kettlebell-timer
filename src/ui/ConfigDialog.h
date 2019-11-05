/**
*  @file ConfigDialog.h
*
*  Config dialog class.
*
*  Author : Anton B. Gusev
**/

#pragma once

#include "ui_ConfigDialog.h"

#include <QSoundEffect>

class Config;

class ConfigDialog : public QDialog, private Ui::ConfigDialog
{
	Q_OBJECT

public:
	explicit ConfigDialog(QWidget *parent = nullptr);

	void SetConfig(Config const& conf);
	void GetConfig(Config& conf);

	static const QString sectNameConnection;
	static const QString varNamePort;

private:
	void playSound(QString const& sound);

	QSoundEffect* effect;
};
