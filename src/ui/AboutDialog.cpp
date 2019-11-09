/**
*  @file AboutDialog.cpp
*
*  About dialog implementation.
*
*  Author : Anton B. Gusev
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
