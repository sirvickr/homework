#pragma once

#include "monitor.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <functional>
#include <list>

void showAll(const std::list<Monitor>& componentRange);

void printList(std::ostream& stream, const std::list<Monitor>& componentRange, std::function<bool(const Monitor& mon)> cond, const std::string& title = "");

void searchByPrice(const std::list<Monitor>& componentRange);

void searchByDiag(const std::list<Monitor>& componentRange);

std::list<Monitor> loadData(const std::string& fileName);

int mainMenu(const std::list<Monitor>& componentRange);

