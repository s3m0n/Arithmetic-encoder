#include "main.h"

int main() {
    setlocale(LC_ALL, "Russian");
    //
    int mode;
    std::cout << "Select mode. For encoding enter 0, for decoding enter 1: ";
    std::cin >> mode;

    std::string decodedFile = "decoded.txt";
    std::string encodedFile = "encoded.txt";

    if (mode) {
        std::string fileName;
        std::cout << "Print name of file, which you want to decode: ";
        std::cin >> fileName;
        decode(fileName, decodedFile);
    }
    else {
        std::string fileName;
        std::cout << "Print name of file, which you want to encode: ";
        std::cin >> fileName;
        encode(fileName, encodedFile);
    }
}