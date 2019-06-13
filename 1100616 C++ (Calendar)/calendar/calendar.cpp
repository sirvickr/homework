#include "calendar.h"
#include "task.h"
#include "birthday.h"
#include "meeting.h"

#include <QDateTime>
#include <QStringList>

static const QString settingsName = "tasks";

Calendar::Calendar() : m_settings("Hyperhard", "Calendar")
{
	int count = m_settings.beginReadArray(settingsName);
	for(int index = 0; index < count; ++index) {
		Task* task = nullptr;
		m_settings.setArrayIndex(index);
		TaskType type = static_cast<TaskType>(m_settings.value("type").toInt());
		QDateTime dt = m_settings.value("when").toDateTime();
		QString name = m_settings.value("name").toString();
		switch(type) {
		case ttCommon:
			task = new Task(dt, name);
			break;
		case ttMeeting:
			if(m_settings.contains("who") && m_settings.contains("where")) {
				task = new Meeting(dt, name,
									 m_settings.value("who").toString(),
									 m_settings.value("where").toString());
			}
			break;
		case ttCall:
			if(m_settings.contains("who")) {
				task = new Interaction(dt, name,
										 m_settings.value("who").toString());
			}
			break;
		case ttBirthday:
			if(m_settings.contains("who") && m_settings.contains("presents")) {
				task = new Birthday(dt, name,
									  m_settings.value("who").toString());
				QStringList strList = m_settings.value("presents").toStringList();
				QList<QString> presents;
				for(const auto& s : strList) {
					static_cast<Birthday*>(task)->addPresent(s);
				}
			}
			break;
		default:
			task = 0;
		}

		if(task) {
			m_tasks.push_back(task);
		}
	}
	m_settings.endArray();
}

Calendar::~Calendar()
{
	m_settings.beginWriteArray(settingsName);

    int index = 0;
	for(auto task : m_tasks) {
        m_settings.setArrayIndex(index++);
		m_settings.setValue("when", task->when());
		m_settings.setValue("name", task->name());
		if(auto meeting = dynamic_cast<const Meeting*>(task)) {
			m_settings.setValue("type", static_cast<int>(ttMeeting));
			m_settings.setValue("who", meeting->who());
			m_settings.setValue("where", meeting->where());
		} else if(auto birthday = dynamic_cast<const Birthday*>(task)) {
			m_settings.setValue("type", static_cast<int>(ttBirthday));
			m_settings.setValue("who", birthday->who());
			QStringList strList;
			const auto presents = birthday->presents();
			for(const auto& present : presents) {
				strList << present;
			}
			m_settings.setValue("presents", strList);
		} else if(auto call = dynamic_cast<const Interaction*>(task)) {
			m_settings.setValue("type", static_cast<int>(ttCall));
			m_settings.setValue("who", call->who());
		} else {
			m_settings.setValue("type", static_cast<int>(ttCommon));
		}
		if(task) {
            delete task;
        }
    }

    m_settings.endArray();
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
    m_tasks.removeAt(index);
    return true;
}

void Calendar::addTask(Task* task)
{
	m_tasks.push_back(task);
}
