#pragma once

#include <iostream>
#include <map>
#include <iomanip>
#include <fstream>

int N = 0;

bool cmpDoubles(long double n1, long double n2) {
	unsigned long long a = (n1 * 1000000000000000000);
	unsigned long long b = (n2 * 1000000000000000000);
	if (a == b) return true;
	return false;
}

std::pair<unsigned char, std::pair<long double, long double>> decodeFunc(long double c, std::map<unsigned char, long double> possibTable, std::pair<long double, long double> borders) {
	long double k = borders.first;
	for (const auto& elem : possibTable) {
		long double oldK = k;
		k = (borders.second - borders.first) * elem.second + k;
		if (c <= k) {
			return std::make_pair(elem.first, std::make_pair(oldK, k));
		}
	}
	return std::make_pair(0, std::make_pair(-1.0, -1.0));
}

std::pair<std::pair<long double, long double>, bool> EncodeFunc(unsigned char a, std::map<unsigned char, long double> possibTable, std::pair<long double, long double> borders) {
	long double k = borders.first;
	for (const auto& elem : possibTable) {
		long double oldK = k;
		k = (borders.second - borders.first) * elem.second + k;
		if (a == elem.first) {
			if(cmpDoubles(oldK, k)) {
				return std::make_pair(std::make_pair(oldK, k), true);
			}
			return std::make_pair(std::make_pair(oldK, k), false);
		}
	}
	return std::make_pair(std::make_pair(-1.0, -1.0), false);
}

void encode(std::string inputFile, std::string outputFile) {
	std::ifstream file(inputFile, std::ios::binary);

	if (!file.is_open()) {
		std::cout << "Error. Input file does not exist" << std::endl;
		return;
	}

	std::map<unsigned char, unsigned int> freqTable;
	std::map<unsigned char, long double> possibTable;
	unsigned int size = 0;

	while (!file.eof()) {
		unsigned char cell = 0;
		file.read((char*)&cell, sizeof(cell));
		freqTable[cell]++;
		size++;
	}

	freqTable[0]--;
	size--;
	if (freqTable[0] == 0) {
		freqTable.erase(0);
	}
	file.close();

	std::ifstream ifs(inputFile, std::ios::binary);
	std::ofstream ofs(outputFile, std::ios::binary);

	unsigned char tableSize = freqTable.size();
	ofs.write((char*)&tableSize, sizeof(char));

	for (const auto& elem : freqTable) {
		possibTable[elem.first] = (long double)elem.second / (long double)size;
		unsigned char let = elem.first;
		unsigned int freq = elem.second;
		ofs.write((char*)&let, sizeof(char));
		ofs.write((char*)&freq, sizeof(freq));
	}

	int counter = 0;
	int x = 0;
	std::pair<std::pair<long double, long double>, bool> borders = std::make_pair(std::make_pair(0.0, 1.0), true);
	int k = 0;
	while (!ifs.eof()) {
		unsigned char cell = 0;
		ifs.read((char*)&cell, sizeof(cell));
		std::pair<std::pair<long double, long double>, bool> newborders = EncodeFunc(cell, possibTable, borders.first);
		counter++;
		
		if ((newborders.second || counter == size) && newborders.first.first != -1.0) {
			long double res = borders.first.second;
			ofs.write((char*)&res, sizeof(res));
			k = 0;
			borders.first.first = 0.0;
			borders.first.second = 1.0;
			newborders = EncodeFunc(cell, possibTable, borders.first);
			borders.first.first = newborders.first.first;
			borders.first.second = newborders.first.second;
		}
		else {
			borders.first.first = newborders.first.first;
			borders.first.second = newborders.first.second;
		}
	}
	ifs.close();
	ofs.close();
}

void decode(std::string inputFile, std::string outputFile) {
	std::ifstream ifs(inputFile, std::ios::binary);
	std::ofstream ofs(outputFile, std::ios::binary);

	if (!ifs.is_open()) {
		std::cout << "Error. Input file does not exist" << std::endl;
		return;
	}

	std::map<unsigned char, unsigned int> freqTable;
	std::map<unsigned char, long double> possibTable;

	unsigned char c;
	ifs.read((char*)&c, sizeof(c));

	for (unsigned char i = 0; i < c; i++) {
		unsigned char let;
		unsigned int freq;
		ifs.read((char*)&let, sizeof(c));
		ifs.read((char*)&freq, sizeof(freq));
		freqTable[let] = freq;
	}

	unsigned int size = 0;
	for (const auto& elem : freqTable) {
		size += elem.second;
	}

	for (const auto& elem : freqTable) {
		possibTable[elem.first] = (long double)elem.second / (long double)size;
	}

	int counter = 0;
	while (!ifs.eof() || counter < size) {
		long double cell;
		ifs.read((char*)&cell, sizeof(cell));
		std::pair<unsigned char, std::pair<long double, long double>> borders = std::make_pair(0, std::make_pair(0.0, 1.0));
		while (counter < size) {
			borders = decodeFunc(cell, possibTable, borders.second);
			ofs.write((char*)&borders.first, sizeof(unsigned char));
			counter++;
			if (cell == borders.second.second) break;
		}
	}
	ifs.close();
	ofs.close();
}