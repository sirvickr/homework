#include "calendar.h"

Calendar::Calendar()
{

}

void Calendar::addTask(const QSharedPointer<Task>& task) {
	m_tasks.push_back(task);
}
