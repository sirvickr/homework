#ifndef TASK_H
#define TASK_H

#include <QString>
#include <QDateTime>

class Task
{
public:
	Task();
	Task(const QDateTime& when, const QString& name);
	QString name() const {
		return m_name;
	}
	void setName(const QString& name) {
		m_name = name;
	}
	QDateTime when() const {
		return m_when;
	}
	void setWhen(const QDateTime& when) {
		m_when = when;
	}
private:
	QString m_name;
	QDateTime m_when;
};

#endif // TASK_H
