#ifndef SPARSEMATRIX_SHORT_DIAGONAL_FORMAT_HPP
#define SPARSEMATRIX_SHORT_DIAGONAL_FORMAT_HPP
#include <algorithm>
#include "packet_matrix_base.hpp"
#include "coord_format_matrix.hpp"

template <size_t diagonalLength>
class CShortDiagonalMatrix : public IPacketMatrixBase {
    struct stDiagonal {
        std::vector<double> data;
        int32_t id;

        stDiagonal &operator=(const stDiagonal &right) {
            data = move(right.data);
            id = right.id;
        }

        void set(const CCoordMatrix::stMatrixElement &data) {
            this->data[data.colId - id] = data.value;
        }

        stDiagonal(int32_t id, const std::vector<double> &data) :
            data(data), id(id) {};

        stDiagonal(int32_t id, const CCoordMatrix::stMatrixElement &data) :
            data(diagonalLength), id(id) {
            set(data);
        };
    };

    struct stDiagonalLines {
        std::vector<stDiagonal> diagonals;
        size_t lineId;

        void insert(int32_t id, const CCoordMatrix::stMatrixElement &data) {
            diagonals.emplace_back(id, data);
        }

        stDiagonalLines(size_t lineId, int32_t id, const CCoordMatrix::stMatrixElement &data) :
            lineId(lineId) {
            this->insert(id, data);
        }
    };

    size_t N;
    std::vector<stDiagonalLines> matrix;

public:
    CShortDiagonalMatrix(const CCoordMatrix &src) {
        loadFromCoord(src);
    }

    size_t getNZ() const {
        size_t count(0);
        int32_t threshold = N - diagonalLength;
        for (const auto &line : matrix) {
            for (const auto &diagonal : line.diagonals) {
                size_t begin((diagonal.id < 0) * -diagonal.id);
                size_t end(diagonalLength - (diagonal.id > threshold ? diagonal.id - threshold : 0));
                for (; begin < end; ++begin)
                    count += diagonal.data[begin] != 0;
            }
        }
        return count;
    }

    size_t getDiagonalLength() const {
        return diagonalLength;
    }

    size_t getM() const {
        size_t count(0);
        for (const auto &line : matrix)
            count += line.diagonals.size();
        return count;
    }

    size_t getN() const {
        return N;
    }

    uint64_t getMatrixSize() const {
        return uint64_t(N) * uint64_t(N);
    }

    void loadFromCoord(const CCoordMatrix &src) {
        N = src.N;
        matrix.clear();
        for (const auto &elem : src.matrix) {
            size_t lineId = elem.rowId / diagonalLength;
            int32_t id = int32_t(elem.colId) - int32_t(elem.rowId % diagonalLength);

            auto line = std::find_if(matrix.begin(), matrix.end(), [&lineId](const stDiagonalLines& l) { return l.lineId == lineId; });
            if (line !=  matrix.end()) {
                auto diagonal = std::find_if(line->diagonals.begin(), line->diagonals.end(), [&id](const stDiagonal& l) { return l.id == id; });
                if (diagonal == line->diagonals.end())
                    line->insert(id, elem);
                else
                    diagonal->set(elem);
            } else
                matrix.emplace_back(lineId, id, elem);
        }
        matrix.shrink_to_fit();
        for (auto& line : matrix)
            std::sort(line.diagonals.begin(), line.diagonals.end(),
                 [](stDiagonal& l, stDiagonal& r)
                 { return l.id < r.id; });
        std::sort(matrix.begin(), matrix.end(),
             [](stDiagonalLines &l, stDiagonalLines &r)
             { return l.lineId < r.lineId; });
    }
    friend class CEvaluation;
};

using msdiag_t32 = CShortDiagonalMatrix<4>;//cache line: 32 bytes (256 bits) = 4 double variable
using msdiag_t64 = CShortDiagonalMatrix<8>;//cache line: 64 bytes (512 bits) = 8 double variable

#endif //SPARSEMATRIX_SHORT_DIAGONAL_FORMAT_HPP
