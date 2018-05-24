#pragma once

template<class Sequence>
void printSequence(const Sequence& seq, const std::string& title = "") {
	std::cout << title;
	for (const auto& item : seq)
		std::cout << " " << item;
	std::cout << std::endl;
}

template<class Sequence>
void printMatrix(const Sequence& matrix, size_t m, size_t n, const std::string& title = "") {
	std::cout << title;
	for (size_t i = 0; i < m; i++) {
		for (size_t j = 0; j < n; j++)
			std::cout << " " << matrix[i * n + j];
		std::cout << std::endl;
	}
}
