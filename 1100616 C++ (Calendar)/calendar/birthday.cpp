#include "birthday.h"

Birthday::Birthday(): Interaction()
{
}

Birthday::Birthday(const QDateTime& when, const QString& name, const QString& who)
	: Interaction(when, name, who)
{
}

QString Birthday::toString() const
{
    QString result = Interaction::toString();
    result += " подарки:";
    for(auto gift : m_presents) {
        result += (" " + gift);
    }
    return result;
}
