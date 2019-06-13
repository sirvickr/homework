#ifndef CALENDAR_H
#define CALENDAR_H

#include <QSettings>
#include <QVector>

class Task;

class Calendar
{
public:
    Calendar();
    ~Calendar();
    Task* task(int index) const;
    bool setTask(int index, Task* task);
    bool delTask(int index);
    void addTask(Task* task);
    int taskCount() const {
        return m_tasks.size();
    }
private:
    QSettings m_settings;
    QVector<Task*> m_tasks;
};

#endif // CALENDAR_H
