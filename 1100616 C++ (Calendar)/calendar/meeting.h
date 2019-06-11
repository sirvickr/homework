#ifndef MEETING_H
#define MEETING_H

#include "interaction.h"

class Meeting : public Interaction
{
public:
	Meeting();
	Meeting(const QDateTime& when, const QString& name, const QString& who, const QString& where);

    QString toString() const override;
    QString where() const {
		return m_where;
	}
	void setWhere(const QString& where) {
		m_where = where;
	}

private:
	QString m_where;
};

#endif // MEETING_H
