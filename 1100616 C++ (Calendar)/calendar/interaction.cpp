#include "interaction.h"

Interaction::Interaction(): Task()
{
}

Interaction::Interaction(const QDateTime& when, const QString& name, const QString& who)
	: Task(when, name), m_who(who)
{
}
