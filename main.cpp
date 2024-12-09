#include <ios>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "bubbleSort.h"
#include "insertionSort.h"
#include "mergeSort.h"

void bubbleSortSimple(long long *A, int n);

bool fileExists(const std::string& path);
void loadData(long long* dataset, const size_t length, std::ifstream& stream);
bool verify(const long long* dataset, const size_t length);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "no data file provided." << std::endl;
        return 1;
    }

    std::string path = argv[1];
    if (!fileExists(path)) {
        std::cout << path << ": file does not exist." << std::endl;
        return 2;
    }

    std::string line;
    std::ifstream file(path);
    std::getline(file, line);
    const int length = std::atoi(line.c_str());

    long long dataset[length];

    std::cout << "loading... " << std::flush;
    loadData(dataset, length, file);
    std::cout << "done.\n";

    std::cout << "verifying... " << std::flush;
    bool sorted = verify(dataset, length);
    std::cout << std::boolalpha << "done. " << (sorted ? "(sorted)" : "(unsorted)") << "\n";

    if (sorted) {
        std::cout << "wdywm to do? lmao" << std::endl;
        return 3;
    }

    long long multi[length];
    std::copy(dataset, dataset + length, multi);

    // sort!
    std::cout << "sorting... (normalbubble) " << std::flush;
    bubbleSortSimple(dataset, length);
    std::cout << "done (" << verify(dataset, length) << ").\n";
    std::cout << "sorting... (multibubble) " << std::flush;
    bubbleSort(multi, length);
    std::cout << "done (" << verify(multi, length) << ").\n";
}

bool fileExists(const std::string& path) {
    struct stat buffer;
    return (stat (path.c_str(), &buffer) == 0);
}

void loadData(long long* dataset, const size_t length, std::ifstream& stream) {
    std::string line;
    int index = 0;
    while (std::getline(stream, line)) {
        dataset[index] = std::stoll(line.c_str());
        ++index;
    }
}

bool verify(const long long* dataset, const size_t length) {
    for (size_t i = 1; i < length; ++i) {
        if (dataset[i - 1] > dataset[i]) return false;
    }
    return true;
}
