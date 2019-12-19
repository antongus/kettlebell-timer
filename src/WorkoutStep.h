/**
*  @file WorkoutStep.h
*
*  WorkoutStep class - single step of workout
*
*  Copyright 2019 Anton B. Gusev
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version. See COPYING file for more details.
**/

#pragma once

#include <QString>
#include <QJsonObject>

struct WorkoutStep
{
	enum class Type
	{
		Comment,  //!< not a step, just comment line
		Pause,    //!< pause
		Work,     //!< work step
		Message,  //!< display message
		Sound,    //!< play sound
	};

	QJsonValue getJson();
	void setJson(const QJsonValue& conf);

	Type type { Type::Pause };     //!< step type
	QString title;                 //!< step title
	qint64 duration { 20 * 1000 }; //!< step duration, ms
	qint64 beeps    { 0 };        //!< beep count

private:

	static QString getNameForType(Type t);
	static Type getTypeFromName(QString name);
};
