#include "coord_format_matrix.hpp"
#include <fstream>
using namespace std;

size_t CCoordMatrix::getNZ() const {
    return matrix.size();
}

size_t CCoordMatrix::getN() const {
    return N;
}

uint64_t CCoordMatrix::getMatrixSize() const {
    return uint64_t(N) * uint64_t(N);
}

size_t CCoordMatrix::generate(size_t N, size_t minInRow, size_t maxInRow, double powerPrecent) {
    size_t NZ(0);
    this->N = N;
    matrix.clear();
    //generate matrix elements
    matrix.shrink_to_fit();
    return NZ;
}

void CCoordMatrix::loadFromMatrixMarketFile(const string& filename) {
    ifstream in(filename);
    string buffer;
    while (getline(in, buffer) &&
        buffer[0] == '%')//ignore comments
        ;
    size_t elementCount(0), m, n;
    if (sscanf(buffer.c_str(), "%u %u %u", &m, &n, &elementCount) && m == n) {
        N = m;
        matrix.clear();
        size_t i, j;
        double value;
        while (elementCount && getline(in, buffer)) {
            sscanf(buffer.c_str(), "%u %u %lf", &i, &j, &value);
            matrix.emplace_back(value, i - 1, j - 1);
            --elementCount;
        }
        matrix.shrink_to_fit();
    }
    in.close();
}