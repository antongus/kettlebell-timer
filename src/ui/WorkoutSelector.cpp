#include "WorkoutSelector.h"

WorkoutSelector::WorkoutSelector(Workouts* workouts, QWidget *parent)
    : QDialog(parent)
    , workouts(workouts)
{
	setupUi(this);

	connect(pbOk, &QPushButton::clicked, [=]{done(QDialog::Accepted); });
	connect(pbCancel, &QPushButton::clicked, [=]{done(QDialog::Rejected); });

	for (auto& workout : *workouts)
	{
		auto item = new QListWidgetItem(QIcon(":/icons/stopwatch.svg"), workout->getTitle(), listWorkouts);
		item->setData(Qt::UserRole, workout->getId());
	}
	listWorkouts->setCurrentRow(0);
}


WorkoutSelector::~WorkoutSelector()
{
}

int WorkoutSelector::getSelectedWorkoutId()
{
	auto item = listWorkouts->currentItem();
	if (item)
	{
		bool ok { false };
		auto id = item->data(Qt::UserRole).toInt(&ok);
		if (ok)
			return id;
	}
	return -1;
}
