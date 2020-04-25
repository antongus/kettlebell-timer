#include "WorkoutSelector.h"

WorkoutSelector::WorkoutSelector(Workouts* workouts, int currentIndex, QWidget *parent)
    : QDialog(parent)
    , workouts(workouts)
{
	setupUi(this);

	connect(pbOk, &QPushButton::clicked, [=]{done(QDialog::Accepted); });
	connect(pbCancel, &QPushButton::clicked, [=]{done(QDialog::Rejected); });
	connect(listWorkouts, &QListWidget::doubleClicked, this, &WorkoutSelector::listDoubleClicked);

	for (auto i = 0u; i < workouts->size(); ++i)
	{
		auto workout = workouts->getWorkout(i);
		auto item = new QListWidgetItem(QIcon(":/icons/stopwatch.svg"), workout->getTitle(), listWorkouts);
		item->setData(Qt::UserRole, workout->getId());
	}
	listWorkouts->setCurrentRow(currentIndex);
}


WorkoutSelector::~WorkoutSelector()
{
}

int WorkoutSelector::getSelectedWorkoutIndex()
{
	return listWorkouts->currentRow();
}

void WorkoutSelector::listDoubleClicked(const QModelIndex &index)
{
	if (index.isValid())
		done(QDialog::Accepted);
}
