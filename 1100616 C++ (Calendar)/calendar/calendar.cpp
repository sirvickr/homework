#include "calendar.h"
#include "task.h"

#include <QDebug>

Calendar::~Calendar()
{
    for(auto task : m_tasks) {
        if(task) {
            delete task;
        }
    }
}

Task* Calendar::task(int index) const
{
    if(index < 0 || index >= taskCount()) {
        return 0;
    }
    return m_tasks[index];
}

bool Calendar::setTask(int index, Task* task)
{
    if(index < 0 || index >= taskCount()) {
        return false;
    }
    if(m_tasks[index]) {
        delete m_tasks[index];
    }
    m_tasks[index] = task;
    return true;
}

bool Calendar::delTask(int index)
{
    if(index < 0 || index >= taskCount()) {
        return false;
    }
    if(m_tasks[index]) {
        delete m_tasks[index];
    }
    qDebug() << "deleting " + QString::number(index) + " count " + QString::number(m_tasks.size());
    m_tasks.removeAt(index);
    qDebug() << "done, count " + QString::number(m_tasks.size());
    return true;
}

void Calendar::addTask(Task* task)
{
	m_tasks.push_back(task);
}
