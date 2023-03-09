#include "main.h"

int main() {
	std::string inputFile = "plain_text.txt";
	std::string outputFile = "ciphertext.txt";
	std::string decodedFile = "decoded.txt";

	encode(inputFile, outputFile);
	decode(outputFile, decodedFile);
}