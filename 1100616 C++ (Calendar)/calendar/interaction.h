#ifndef INTERACTION_H
#define INTERACTION_H

#include "task.h"

class Interaction : public Task
{
public:
	Interaction();
	Interaction(const QDateTime& when, const QString& name, const QString& who);
    QString toString() const override;
    QString who() const {
		return m_who;
	}
	void setWho(const QString& who) {
		m_who = who;
	}
private:
	QString m_who;
};

#endif // INTERACTION_H
