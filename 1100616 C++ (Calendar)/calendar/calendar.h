#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMap>
#include <QSharedPointer>
#include <QList>

class Task;

class Calendar
{
public:
	Calendar();
	void addTask(const QSharedPointer<Task>& task);
private:
	QList<QSharedPointer<Task>> m_tasks;
};

#endif // CALENDAR_H
