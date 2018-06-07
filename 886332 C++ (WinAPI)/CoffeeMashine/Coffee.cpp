#include "stdafx.h"

#include "Coffee.h"

Coffee::Coffee(const std::string& kind, double price, int count) : kind(kind), price(price), count(count)
{
}

int Coffee::takeSome(int count)
{
	if (this->count < count)
		throw NotEnough(this->count);
	this->count -= count;
	return this->count;
}

int Coffee::addSome(int count)
{
	this->count += count;
	return this->count;
}
