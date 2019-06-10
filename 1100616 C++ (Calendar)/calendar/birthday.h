#ifndef BIRTHDAY_H
#define BIRTHDAY_H

#include "interaction.h"
#include <QList>

class Birthday : public Interaction
{
public:
	Birthday();
	Birthday(const QDateTime& when, const QString& name, const QString& who);
	QList<QString> presents() const {
		return m_presents;
	}
	void addPresent(const QString& present) {
		m_presents.push_back(present);
	}
private:
	QList<QString> m_presents;
};

#endif // BIRTHDAY_H
