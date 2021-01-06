#ifndef INVENTION_H
#define INVENTION_H

#include <QString>
#include <QVector>
#include <QDate>
#include <QDataStream>
#include <tuple>

// Открытие/изобретение
class Invention
{
	// Названия областей дейтельности
	static const QVector<QString> RealmNames;
public:
	// Перечень областей дейтельности
	enum Realm { Unspecified, Electroncs, Chemistry, Physics, Biology, Maths, enum_size };
	// "Вес" экземпляра, для сравнения экземпляров
	using Rank = std::tuple<Realm, int, const QString&, const QString&, bool, bool, QDate>;

	Invention() = default;
	Invention(Realm realm, const QString& name, int year, const QString& authors, bool award, bool patent, QDate patentDate);

	// Методы доступа

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

	inline const Rank rank() const {
		return { mRealm, mYear, mName, mAuthors, mAward, mPatent, mPatentDate };
	}

	// Операторы сравнения

	inline bool operator<(const Invention& rhs) const {
		return this->rank() < rhs.rank();
	}

	inline bool operator<=(const Invention& rhs) const {
		return this->rank() <= rhs.rank();
	}

	inline bool operator>=(const Invention& rhs) const {
		return this->rank() >= rhs.rank();
	}

	inline bool operator>(const Invention& rhs) const {
		return this->rank() > rhs.rank();
	}

	inline bool operator==(const Invention& rhs) const {
		return this->rank() == rhs.rank();
	}

	inline bool operator!=(const Invention& rhs) const {
		return !(*this == rhs);
	}

	friend QDataStream& operator <<(QDataStream& stream, const Invention& instance)
	{
		stream << instance.mRealm;
		stream << instance.mYear;
		stream << instance.mName;
		stream << instance.mAuthors;
		stream << instance.mAward;
		stream << instance.mPatent;
		stream << instance.mPatentDate;
		return stream;
	}

	friend QDataStream& operator >>(QDataStream& stream, Invention& instance)
	{
		stream >> instance.mRealm;
		stream >> instance.mYear;
		stream >> instance.mName;
		stream >> instance.mAuthors;
		stream >> instance.mAward;
		stream >> instance.mPatent;
		stream >> instance.mPatentDate;
		return stream;
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
	QDate mPatentDate = QDate(1900, 1, 1);
};

#endif // INVENTION_H
