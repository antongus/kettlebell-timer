/**
*  @file AboutDialog.h
*
*  About dialog class.
*
*  Author : Anton B. Gusev
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
