#include "invention.h"

const QVector<QString> Invention::RealmNames {
	"Неизвестна", "Электроника", "Химия", "Физика", "Биология", "Математика"
};

Invention::Invention(Realm realm, const QString& name, int year, const QString& authors, bool award, bool patent, QDate patentDate)
	: mRealm(realm), mName(name), mYear(year), mAuthors(authors), mAward(award), mPatent(patent), mPatentDate(patentDate)
{
}
