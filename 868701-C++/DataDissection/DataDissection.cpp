#include "stdafx.h"

#include <iostream>       // std::cin, std::cout
#include <sstream>        // std::ostringstream
#include <string>         // std::string
#include <vector>         // std::vector
#include <list>           // std::list
#include <fstream>        // std::ifstream, std::ofstream
#include <algorithm>      // std::random_shuffle

using namespace std;
// �������
static const size_t blockCount = 15; // ���������� ������
static const size_t colCount = 5; // ���������� �������� (����� ����� �� ��������)
static const size_t rowCount = blockCount / colCount; // ����� ����� �� �������

// ������������������ ��������
typedef vector<uint8_t> data_t;
// ������� ���� ������
typedef list<uint8_t> block_t;
// ������ ������ (��������� �����������)
typedef vector<block_t> blocks_t;
// ���� (������������������ ��������)
typedef vector<size_t> key_t;

char* readFile(const string& rawFileName, size_t& length, bool binary = false) {
	length = 0;
	char* lpBuf = 0;
	ios_base::openmode mode = ios::in;
	if (binary)
		mode |= ios::binary;
	ifstream input(rawFileName, mode);
	if (!input) {
		cerr << "�� ������� ������� ���� \"" << rawFileName << "\"" << endl;
		return nullptr;
	}
	input.seekg(0, ios::end);
	length = input.tellg();
	input.seekg(0, ios::beg);

	if (0 == length) {
		cerr << "���� \"" << rawFileName << "\" ����" << endl;
		return NULL;
	}
	cout << "sizeof \"" << rawFileName << "\" = " << length << endl;
	lpBuf = new char[length + 1];
	input.read(lpBuf, length);
	lpBuf[length] = '\0';
	return lpBuf;
}

blocks_t encode(const data_t& data, const key_t& colKey, const key_t& rowKey) {
	blocks_t blocks(blockCount);

	for (size_t i = 0; i < data.size(); i++) {
		size_t row = i / colCount;
		size_t col = i % colCount;
		size_t k = colCount * rowKey[row % rowCount] + colKey[col];
		blocks[k].push_back(data[i]);
	}

	return blocks;
}

void encode(const string& rawFileName, const string& encFileName, const key_t& colKey, const key_t& rowKey) {
	size_t length = 0;
	char* lpBuf = readFile(rawFileName, length);
	if (!lpBuf)
		return;

	cout << "\n�����:\n" << lpBuf << endl << endl;

	data_t data(lpBuf, lpBuf + length);

	blocks_t blocks = encode(data, colKey, rowKey);
	
	ofstream output(encFileName, ios::binary);
	if (!output) {
		cerr << "�� ������� ������� ���� \"" << encFileName << "\"" << endl;
		return;
	}

	cout << "�������������� �����:" << endl;
	for (const auto& block : blocks) {
		cout << string(block.begin(), block.end()) << endl;
		// ���������� ������ ���������� �����
		uint16_t blockSize = block.size();
		output.write((const char*)&blockSize, sizeof(blockSize));
		// ���������� ����� ���������� �����
		for (const auto byte : block) {
			output.write((const char*)&byte, sizeof(byte));
		}
	}
}

data_t decode(blocks_t& blocks, const key_t& colKey, const key_t& rowKey) {
	size_t size = 0;
	for (const auto& block : blocks)
		size += block.size();
	cout << "��������� ������ ������: " << size << endl;
	
	data_t data(size, 0);
	for (size_t i = 0; i < data.size(); i++) {
		size_t row = i / colCount;
		size_t col = i % colCount;
		size_t k = colCount * rowKey[row % rowCount] + colKey[col];
		if (!blocks[k].empty()) {
			data[i] = blocks[k].front();
			blocks[k].pop_front();
		}
	}
	return data;
}

void decode(const string& rawFileName, const string& encFileName, const key_t& colKey, const key_t& rowKey) {
	size_t length = 0;
	char* lpBuf = readFile(encFileName, length, true);
	if (!lpBuf)
		return;

	blocks_t blocks;

	for (char* lpCurr = lpBuf; lpCurr - lpBuf < length; ) {
		uint16_t blockSize = *(uint16_t*)lpCurr;
		lpCurr += sizeof(blockSize);
		block_t block;
		for (auto j = 0; j < blockSize; j++) {
			block.push_back(*lpCurr++);
		}
		blocks.push_back(block);
	}
	
	data_t data = decode(blocks, colKey, rowKey);
	string text = string(data.begin(), data.end());
	cout << "\n�����:\n" << text << endl << endl;

	ofstream rawFile(rawFileName);
	if (!rawFile) {
		cerr << "�� ������� ������� ���� \"" << rawFileName << "\"" << endl;
		return;
	}

	rawFile << text;
}

string keyStr(const key_t& key, size_t count) {
	ostringstream os;
	for (size_t i = 0; i < count; ++i)
		os << " " << key[i];
	return os.str();
}

void keyReq(key_t& key, size_t count, const string& prompt) {
	cout << prompt;
	for (size_t i = 0; i < count; ++i) {
		cin >> key[i];
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	string rawFileName = "raw.txt";
	string encFileName = "encoded.txt";

	// ����� �� �������� � �������:
	// ������������������� �� ���������, ������ ��� �������,
	// ����� �������������� ��������� ������� � std::random_shuffle()
	key_t colKey { 3, 0, 2, 1, 4 };
	key_t rowKey { 1, 0, 3, 2, 6, 5, 7, 6 };
#if 1
	for (int i = 0; i < colCount; ++i)
		colKey[i] = i;
	random_shuffle(colKey.begin(), colKey.begin() + colCount);
	for (int i = 0; i < rowCount; ++i)
		rowKey[i] = i;
	random_shuffle(rowKey.begin(), rowKey.begin() + rowCount);
#endif
	cout << "��������� ���������: blockCount " << blockCount
		<< " colCount " << colCount << " rowCount " << rowCount;
	cout << " colKey " << keyStr(colKey, colCount) << " rowKey " << keyStr(rowKey, rowCount) << endl;

	int menu = -1;
	while (menu != 0) {
		cout << endl << endl << "�������� ��������:" << endl;
		cout << "[0] ���������� ������" << endl;
		cout << "[1] ������� ���� � ��������� ������� ( " << rawFileName << " )" << endl;
		cout << "[2] ������� ���� ��� ������ ���������� ( " << encFileName << " )" << endl;
		cout << "[3] ������� ���� �� ������� (" << keyStr(rowKey, rowCount) << " )" << endl;
		cout << "[4] ������� ���� �� �������� (" << keyStr(colKey, colCount) << " )" << endl;
		cout << "[5] ������������ ������" << endl;
		cout << "[6] ������������� ������" << endl;

		cout << endl;

		cout << "��� �����: ";
		cin.sync();
		cin >> menu;

		blocks_t blocks;

		switch (menu) {
		case 1:
			cout << "������� ��� ����� � ��������� �������: ";
			cin >> rawFileName;
			break;
		case 2:
			cout << "������� ��� ����� ��� ������ ����������: ";
			cin >> encFileName;
			break;
		case 3:
			keyReq(rowKey, rowCount, "������� ���� �� �������: ");
			break;
		case 4:
			keyReq(colKey, colCount, "������� ���� �� ��������: ");
			break;
		case 5:
			encode(rawFileName, encFileName, colKey, rowKey);
			break;
		case 6:
			decode(rawFileName, encFileName, colKey, rowKey);
			break;
		default:
			menu = 0;
		}
	}
	
	return 0;
}
