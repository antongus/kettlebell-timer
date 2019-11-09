/**
*  @file AboutDialog.h
*
*  About dialog class.
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

#include <QDialog>
#include "ui_AboutDialog.h"

class AboutDialog : public QDialog, private Ui::AboutDialog
{
	Q_OBJECT
public:
	explicit AboutDialog(QWidget* parent = nullptr);
private:
	void InsertVersion();
};
