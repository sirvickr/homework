#pragma once

#include <ostream>
#include <istream>
#include <sstream>

template<typename T>
T input_impl(std::istream& is, std::ostream& os, const T* def_val, const char* prompt) {
	if(prompt) {
		os << prompt;
		if(def_val)
			os << " [" << *def_val << "]";
		os << ": ";
	}
	T value;
	std::string s;
	if((is >> s) && s.size() > 0) {
		std::istringstream iss(s);
		iss >> value;
		return value;
	}
	if(def_val) {
		return *def_val;
	}
	return T();
}

template<typename T>
T input(std::istream& is, std::ostream& os, const char* prompt = nullptr) {
	return input_impl(is, os, static_cast<T*>(nullptr), prompt);
}

template<typename T>
T input(std::istream& is, std::ostream& os, const T& def_val, const char* prompt = nullptr) {
	return input_impl(is, os, &def_val, prompt);
}
