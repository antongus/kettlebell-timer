/**
*  @file AboutDialog.cpp
*
*  About dialog implementation.
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#include "AboutDialog.h"
#include <QDate>

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	textBrowser->viewport()->setAutoFillBackground(false);
	InsertVersion();
	setFixedSize(size());
	connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
}

void AboutDialog::InsertVersion()
{
	auto cursor = textBrowser->document()->find("%%VERSION%%");
	cursor.insertText(tr("version: %1").arg(QApplication::applicationVersion()));
}
