#ifndef INVENTION_H
#define INVENTION_H

#include <QString>
#include <QVector>
#include <QDate>

// Открытие/изобретение
class Invention
{
	static const QVector<QString> RealmNames;
public:
	enum Realm { Unspecified, Electroncs, Chemistry, Physics, Biology, Maths, enum_size };

	Invention() = default;
	Invention(Realm realm, const QString& name, int year, const QString& authors, bool award, bool patent, QDate patentDate);

	static const QString& realmName(Realm realm) {
		return RealmNames[realm];
	}
	Realm realm() const noexcept {
		return mRealm;
	}
	void realm(Realm value) noexcept {
		mRealm = value;
	}
	const QString& name() const noexcept {
		return mName;
	}
	void name(const QString& value) noexcept {
		mName = value;
	}

	int year() const noexcept {
		return mYear;
	}
	void year(int value) noexcept {
		mYear = value;
	}

	const QString& authors() const noexcept {
		return mAuthors;
	}
	void authors(const QString& value) noexcept {
		mAuthors = value;
	}
	void addAuthor(const QString& value) noexcept {
		mAuthors += value;
	}

	bool award() const noexcept {
		return mAward;
	}
	void award(bool value) noexcept {
		mAward = value;
	}

	bool patent() const noexcept {
		return mPatent;
	}
	void patent(bool value) noexcept {
		mPatent = value;
	}

	QDate patentDate() const noexcept {
		return mPatentDate;
	}
	void patentDate(QDate value) noexcept {
		mPatentDate = value;
	}

private:
	// Область применения
	Realm mRealm = Realm::Unspecified;
	// Наименование
	QString mName = "";
	// Год
	int mYear = 1900;
	// Авторы
	QString mAuthors = "";
	// Присуждена нобелевская премия
	bool mAward = false;
	// Наличие патента
	bool mPatent = false;
	// Дата регистрации патента
	QDate mPatentDate = QDate::currentDate();
};

#endif // INVENTION_H
