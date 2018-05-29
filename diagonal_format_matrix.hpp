#ifndef SPARSEMATRIX_DIAGONAL_FORMAT_MATRIX_HPP
#define SPARSEMATRIX_DIAGONAL_FORMAT_MATRIX_HPP
#include "packet_matrix_base.hpp"
#include "coord_format_matrix.hpp"

class CCoordMatrix;

class CDiagonalMatrix : public IPacketMatrixBase {
    struct stDiagonal {
        std::vector<double> data;
        int32_t id;

        stDiagonal& operator=(const stDiagonal& right);
        void set(const CCoordMatrix::stMatrixElement& data);

        stDiagonal(int32_t  id, const std::vector<double>& data) :
            data(data), id(id) {};

        stDiagonal(int32_t id, const CCoordMatrix::stMatrixElement& data, size_t length)
            : data(length), id(id) {
            set(data);
        };
    };

    size_t N;
    std::vector<stDiagonal> matrix;

public:
    CDiagonalMatrix(const CCoordMatrix& src) {
        loadFromCoord(src);
    }
    size_t getM() const;
    size_t getNZ() const;
    size_t getN() const;
    uint64_t getMatrixSize() const;
    void loadFromCoord(const CCoordMatrix& src);
    friend class CEvaluation;
};

using mdiag_t = CDiagonalMatrix;

#endif //SPARSEMATRIX_DIAGONAL_FORMAT_MATRIX_HPP
