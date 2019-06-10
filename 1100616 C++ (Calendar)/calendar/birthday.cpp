#include "birthday.h"

Birthday::Birthday(): Interaction()
{
}

Birthday::Birthday(const QDateTime& when, const QString& name, const QString& who)
	: Interaction(when, name, who)
{
}
