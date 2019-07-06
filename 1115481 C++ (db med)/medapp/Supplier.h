#pragma once

#include "NamedItem.h"

class Supplier : public NamedItem
{
public:
	Supplier() = default;
	Supplier(int id, const std::string& name) : NamedItem(id, name) {
	}
	~Supplier() = default;
};

