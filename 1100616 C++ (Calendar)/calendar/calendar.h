#ifndef CALENDAR_H
#define CALENDAR_H

#include <QMap>
#include <QVector>

class Task;

class Calendar
{
public:
    Calendar() = default;
    ~Calendar();
    Task* task(int index) const;
    bool setTask(int index, Task* task);
    bool delTask(int index);
    void addTask(Task* task);
    int taskCount() const {
        return m_tasks.size();
    }
private:
    QVector<Task*> m_tasks;
};

#endif // CALENDAR_H
