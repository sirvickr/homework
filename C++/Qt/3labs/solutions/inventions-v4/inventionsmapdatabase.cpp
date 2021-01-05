#include "inventionsmapdatabase.h"

InventionsMapDatabase::InventionsMapDatabase()
{

}

int InventionsMapDatabase::count() const
{
	return inventions.size();
}

int InventionsMapDatabase::append(const Invention& record)
{
#if 1
	Id id = nextID++;
	inventions[id] = record;
	return id;
#else
	mCurrId = nextID++;
	inventions[mCurrId] = record;
	return mCurrId;
#endif
}

/*int InventionsMapDatabase::append()
{
	return append(mBuffer);
}*/

void InventionsMapDatabase::remove(Id id)
{
	inventions.remove(id);
}

/*void InventionsMapDatabase::remove()
{
	remove(mCurrId);
}*/

void InventionsMapDatabase::update(Id id, const Invention& record)
{
	auto item = inventions.find(id);
	if(item == inventions.end())
		return;
	item.value() = record;
}

/*void InventionsMapDatabase::update()
{
	auto item = inventions.find(mCurrId);
	if(item == inventions.end())
		return;
	item.value() = mBuffer;
}*/

bool InventionsMapDatabase::record(Id id, Invention &record) const
{
	auto item = inventions.find(id);
	if(item == inventions.end())
		return false;
	record = item.value();
	return true;
}

/*InventionsMapDatabase::Id InventionsMapDatabase::current() const {
	return mCurrId;
}

void InventionsMapDatabase::current(Id id) {
	auto item = inventions.find(id);
	if(item == inventions.end())
		return;
	mCurrId = id;
	mBuffer = item.value();
}*/

bool InventionsMapDatabase::save(const QString& filename) const
{
	return false;
}

bool InventionsMapDatabase::load(const QString& filename)
{
	return false;
}

void InventionsMapDatabase::clear() {
}

bool InventionsMapDatabase::isModified() const
{
	return false;
}

/*bool InventionsMapDatabase::isBufferModified(Id id) const
{
	auto item = inventions.find(id);
	if(item == inventions.end())
		return false;
	return mBuffer == item.value();
}*/
