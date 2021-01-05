#ifndef INVENTION_H
#define INVENTION_H

#include <QString>
#include <QVector>
#include <QDate>
#include <tuple>

// Открытие/изобретение
class Invention
{
	static const QVector<QString> RealmNames;
public:
	enum Realm { Unspecified, Electroncs, Chemistry, Physics, Biology, Maths, enum_size };

	Invention() = default;
	Invention(Realm realm, const QString& name, int year, const QString& authors, bool award, bool patent, QDate patentDate = QDate(1900, 1, 1));

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

	inline const std::tuple<Realm, int, const QString&> shortRank() const {
		return { mRealm, mYear, mName };
	}

	inline const std::tuple<Realm, const QString&, int, const QString&, bool, bool, QDate> fullRank() const {
		return { mRealm, mName, mYear, mAuthors, mAward, mPatent, mPatentDate };
	}

	// операторы сравнения используются для сортировки
	inline bool operator<(const Invention& rhs) const {
		return this->shortRank() < rhs.shortRank();
	}

	inline bool operator<=(const Invention& rhs) const {
		return this->shortRank() <= rhs.shortRank();
	}

	inline bool operator>=(const Invention& rhs) const {
		return this->shortRank() >= rhs.shortRank();
	}

	inline bool operator>(const Invention& rhs) const {
		return this->shortRank() > rhs.shortRank();
	}

	// операторы == и != сравнивают все поля
	inline bool operator==(const Invention& rhs) const {
		return this->fullRank() == rhs.fullRank();
	}

	inline bool operator!=(const Invention& rhs) const {
		return !(*this == rhs);
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
