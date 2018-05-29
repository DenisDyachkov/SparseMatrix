#include "diagonal_format_matrix.hpp"
#include <algorithm>
using namespace std;

CDiagonalMatrix::stDiagonal& CDiagonalMatrix::stDiagonal::operator=(const stDiagonal& right) {
    data = move(right.data);
    id = right.id;
}

void CDiagonalMatrix::stDiagonal::set(const CCoordMatrix::stMatrixElement& data) {
    this->data[data.colId - id] = data.value;
}

size_t CDiagonalMatrix::getM() const {
    return matrix.size();
}

size_t CDiagonalMatrix::getNZ() const {
    size_t count(0);
    for (const auto &diagonal : matrix) {
        size_t begin((diagonal.id < 0) * -diagonal.id);
        size_t end(N - (diagonal.id > 0) * diagonal.id);
        for (; begin < end; ++begin)
            count += diagonal.data[begin] != 0;
    }
    return count;
}

size_t CDiagonalMatrix::getN() const {
    return N;
}

uint64_t CDiagonalMatrix::getMatrixSize() const {
    return uint64_t(N) * uint64_t(N);
}

void CDiagonalMatrix::loadFromCoord(const CCoordMatrix& src) {
    N = src.N;
    matrix.clear();
    for (const auto &elem : src.matrix) {
        int32_t id = int32_t(elem.colId) - int32_t(elem.rowId);
        auto diagonal = find_if(matrix.begin(), matrix.end(), [&id](const stDiagonal &l)
        { return l.id == id; });
        if (diagonal == matrix.end())
            matrix.emplace_back(id, elem, N);
        else
            diagonal->set(elem);
    }
    matrix.shrink_to_fit();
    sort(matrix.begin(), matrix.end(),
         [](stDiagonal &l, stDiagonal &r)
         { return l.id < r.id; });
}