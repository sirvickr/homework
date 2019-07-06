#pragma once

#include "NamedItem.h"

class Client : public NamedItem
{
public:
	Client() = default;
	Client(int id, const std::string& name) : NamedItem(id, name) {
	}
	~Client() = default;
};

