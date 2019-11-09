/**
*  @file ConfigDialog.h
*
*  Config dialog class.
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
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
