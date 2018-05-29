#ifndef SPARSEMATRIX_EVALUTAION_HPP
#define SPARSEMATRIX_EVALUTAION_HPP

#include "coord_format_matrix.hpp"
#include "diagonal_format_matrix.hpp"
#include "short_diagonal_format.hpp"
#include <iostream>
#include <iomanip>

class CEvaluation {
    static void outBaseInformation(const IPacketMatrixBase& matrix, const std::string& type);
    static void coordMatrix(const CCoordMatrix& matrix);
    static void diagonalMatrix(const CDiagonalMatrix& matrix);
public:
    static void storageEfficiency(const IPacketMatrixBase& matrix);

    template<size_t size>
    static void storageEfficiency(const CShortDiagonalMatrix<size>& matrix) {
        outBaseInformation(matrix, "Short-diagonal");
        std::cout << "Length of short diagonal: " << size << " elements" << std::endl;
        std::cout << "Number of nonzero short diagonals: " << matrix.getM()
             << " [" << matrix.getM() * 100.0 / ((matrix.getN() + size - 1) * matrix.getN() / size) << "%]" << std::endl;
        size_t sizeOfValue = sizeof(matrix.matrix[0].diagonals[0].data[0]);
        size_t sizeIndex = sizeof(CShortDiagonalMatrix<size>::stDiagonal::id);
        size_t sizeLine = sizeof(CShortDiagonalMatrix<size>::stDiagonalLines::lineId);
        size_t sizeOfLines = sizeLine * matrix.matrix.size();
        size_t sizeDiagIndex = sizeIndex * matrix.getM();
        size_t sizeOfElements = sizeOfValue * matrix.getNZ();
        uint64_t totalSizeUsed = sizeOfValue * size * matrix.getM() + sizeDiagIndex + sizeOfLines;
        size_t sizeOfGarbage(0);
        int32_t threshold = matrix.getN() - size;
        for (const auto &line : matrix.matrix) {
            size_t addition = ((line.lineId + 1) * size > matrix.getN() ? (line.lineId + 1) * size - matrix.getN() : 0);
            for (const auto &diagonal : line.diagonals) {
                size_t count = (diagonal.id < 0 ? addition - diagonal.id : (diagonal.id > threshold ? std::max(size_t(diagonal.id - threshold), addition) : addition));
                sizeOfGarbage += count * sizeOfValue;
            }
        }
        uint64_t sizeOfZero = totalSizeUsed - sizeOfElements - sizeDiagIndex - sizeOfGarbage - sizeOfLines;
        std::cout << std::setprecision(3)
             << ">Information about the memory used:" << '\n'
             << " Summary memory used for storage: " << totalSizeUsed << " bytes\n"
             << " The memory used to store nonzero elements: " << sizeOfElements << " bytes [" << sizeOfElements * 100.0 / totalSizeUsed << "%]" << '\n'
             << " The memory used to store id diagonals: " << sizeDiagIndex << " bytes [" << sizeDiagIndex * 100.0 / totalSizeUsed << "%]" << '\n'
             << " Memory used to store zero items: " << sizeOfZero << " bytes [" << sizeOfZero * 100.0 / totalSizeUsed << "%]" << '\n'
             << " Memory used for garbage storage: " << sizeOfGarbage << " bytes [" << sizeOfGarbage * 100.0 / totalSizeUsed << "%]" << '\n'
             << " The memory used to store id lines: " << sizeOfLines << " bytes [" << sizeOfLines * 100.0 / totalSizeUsed << "%]" << std::endl;
    }
};

#endif //SPARSEMATRIX_EVALUTAION_HPP
