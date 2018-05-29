#ifndef SPARSEMATRIX_COORD_FORMAT_MATRIX_HPP
#define SPARSEMATRIX_COORD_FORMAT_MATRIX_HPP

#include "packet_matrix_base.hpp"
#include <string>

class CCoordMatrix : public IPacketMatrixBase {
    struct stMatrixElement {
        double value;
        size_t rowId;
        size_t colId;

        stMatrixElement(double value, size_t rowId, size_t colId) :
            value(value), rowId(rowId), colId(colId) {};
    };
    size_t N;
    std::vector<stMatrixElement> matrix;

public:
    size_t getNZ() const;
    size_t getN() const;
    uint64_t getMatrixSize() const;
    size_t generate(size_t N, size_t minInRow, size_t maxInRow, double powerPrecent);
    void loadFromMatrixMarketFile(const std::string& filename);
    template <size_t N>
    void init(const double (&initMatrix)[N][N]) {
        this->N = N;
        matrix.clear();
        for (size_t i = 0; i < N; ++i)
            for (size_t j = 0; j < N; ++j)
                if (initMatrix[i][j] != 0)
                    matrix.emplace_back(initMatrix[i][j], i, j);
        matrix.shrink_to_fit();
    }

    friend class CDiagonalMatrix;
    template <size_t> friend class CShortDiagonalMatrix;
    friend class CEvaluation;
};

using mcoord_t = CCoordMatrix;

#endif //SPARSEMATRIX_COORD_FORMAT_MATRIX_HPP
