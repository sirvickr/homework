#include "task.h"

Task::Task()
{
}

Task::Task(const QDateTime& when, const QString& name)
	: m_name(name), m_when(when)
{
}

QString Task::toString() const
{
    return m_when.toString() + " " + m_name;
}
