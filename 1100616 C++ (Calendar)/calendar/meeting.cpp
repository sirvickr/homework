#include "meeting.h"

Meeting::Meeting(): Interaction()
{
}

Meeting::Meeting(const QDateTime& when, const QString& name, const QString& who, const QString& where)
	: Interaction(when, name, who), m_where(where)
{
}

QString Meeting::toString() const
{
    return Interaction::toString() + " место: " + where();
}
