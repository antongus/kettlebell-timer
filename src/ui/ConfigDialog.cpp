/**
*  @file ConfigDialog.cpp
*
*  Config dialog implementation.
*
*  Author : Anton B. Gusev
**/

#include "ConfigDialog.h"
#include "Application.h"
#include "Config.h"

#include <QDir>
#include <QSoundEffect>

ConfigDialog::ConfigDialog(QWidget *parent) :
	QDialog(parent)
{
	setupUi(this);
	effect = new QSoundEffect(this);

	QDir dir(Application::instanse()->getSoundsDir());
	auto files = dir.entryList(QDir::Files);

	cbCountdownSound->addItems(files);
	cbStartSound->addItems(files);
	cbFinishSound->addItems(files);
	cbMetronomSound->addItems(files);
	connect(tbPlay1, &QToolButton::clicked, [&](){ playSound(cbCountdownSound->currentText()); });
	connect(tbPlay2, &QToolButton::clicked, [&](){ playSound(cbStartSound->currentText()); });
	connect(tbPlay3, &QToolButton::clicked, [&](){ playSound(cbFinishSound->currentText()); });
	connect(tbPlay4, &QToolButton::clicked, [&](){ playSound(cbMetronomSound->currentText()); });
}

void ConfigDialog::SetConfig(Config const& conf)
{
	auto setValue = [](QComboBox* combo, QString const& value){
		auto pos = combo->findText(value);
		if (pos < 0)
			combo->setCurrentIndex(-1);
		else
			combo->setCurrentIndex(pos);
	};
	setValue(cbCountdownSound, conf.countdownSoundFileName);
	setValue(cbStartSound, conf.startSoundFileName);
	setValue(cbFinishSound, conf.finishSoundFileName);
	setValue(cbMetronomSound, conf.metronomSoundFileName);
}

void ConfigDialog::GetConfig(Config& conf)
{
	conf.countdownSoundFileName = cbCountdownSound->currentText();
	conf.startSoundFileName = cbStartSound->currentText();
	conf.finishSoundFileName = cbFinishSound->currentText();
	conf.metronomSoundFileName = cbMetronomSound->currentText();
}

void ConfigDialog::playSound(const QString& sound)
{
	QDir dir(Application::instanse()->getSoundsDir());
	effect->setSource(QUrl::fromLocalFile(dir.filePath(sound)));
	effect->play();
}
