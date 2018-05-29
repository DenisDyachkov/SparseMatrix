#include "evalutaion.hpp"
using namespace std;

void CEvaluation::outBaseInformation(const IPacketMatrixBase& matrix, const string& type) {
    cout << setprecision(3)
         << "======>  " << type << " matrix storage format  <======" << '\n'
         << "Matrix size: " << matrix.getN() << 'x' << matrix.getN()
         << " (" << matrix.getMatrixSize() << " elements)" << '\n'
         << "Number of nonzero elements: " << matrix.getNZ()
         << " [" << matrix.getNZ() * 100.0 / matrix.getMatrixSize() << "%]" << endl;
}

void CEvaluation::coordMatrix(const CCoordMatrix& matrix) {
    outBaseInformation(matrix, "Coordinate");
    size_t sizeOfValue = sizeof(CCoordMatrix::stMatrixElement::value);
    size_t sizeIndex = sizeof(CCoordMatrix::stMatrixElement::rowId);
    size_t sizeOfElements = sizeOfValue * matrix.getNZ();
    size_t sizePosition = 2 * sizeIndex * matrix.getNZ();
    size_t totalSizeUsed = sizeOfElements + sizePosition;
    cout << setprecision(3)
         << ">Information about the memory used:" << '\n'
         << " Summary memory used for storage: " << totalSizeUsed << " bytes\n"
         << " The memory used to store nonzero elements: " << sizeOfElements
         << " bytes [" << sizeOfElements * 100.0 / totalSizeUsed << "%]" << '\n'
         << " The memory used to store index elements: " << sizePosition
         << " bytes [" << sizePosition * 100.0 / totalSizeUsed << "%]" << endl;
}

void CEvaluation::diagonalMatrix(const CDiagonalMatrix& matrix) {
    outBaseInformation(matrix, "Diagonal");
    cout << "Number of nonzero diagonals: " << matrix.getM()
         << " [" << matrix.getM() * 100.0 / (matrix.getN() * 2 - 1) << "%]" << endl;
    size_t sizeOfValue = sizeof(matrix.matrix[0].data[0]);
    size_t sizeIndex = sizeof(CDiagonalMatrix::stDiagonal::id);
    size_t sizeOfElements = sizeOfValue * matrix.getNZ();
    size_t sizeDiagIndex = sizeIndex * matrix.getM();
    uint64_t totalSizeUsed = sizeOfValue * matrix.getN() * matrix.getM() + sizeDiagIndex;
    size_t sizeOfGarbage(0);
    for (const auto& diagonal : matrix.matrix)
        sizeOfGarbage += abs(diagonal.id) * sizeOfValue;
    uint64_t sizeOfZero = totalSizeUsed - sizeOfElements - sizeDiagIndex - sizeOfGarbage;
    cout << setprecision(3)
         << ">Information about the memory used:" << '\n'
         << " Summary memory used for storage: " << totalSizeUsed << " bytes\n"
         << " The memory used to store nonzero elements: " << sizeOfElements << " bytes [" << sizeOfElements * 100.0 / totalSizeUsed << "%]" << '\n'
         << " The memory used to store id diagonals: " << sizeDiagIndex << " bytes [" << sizeDiagIndex * 100.0 / totalSizeUsed << "%]" << '\n'
         << " Memory used to store zero items: " << sizeOfZero << " bytes [" << sizeOfZero * 100.0 / totalSizeUsed << "%]" << '\n'
         << " Memory used for garbage storage: " << sizeOfGarbage << " bytes [" << sizeOfGarbage * 100.0 / totalSizeUsed << "%]" << endl;
}

void CEvaluation::storageEfficiency(const IPacketMatrixBase& matrix) {
    if (dynamic_cast<const CCoordMatrix*>(&matrix))
        coordMatrix(dynamic_cast<const CCoordMatrix&>(matrix));
    else if (dynamic_cast<const CDiagonalMatrix*>(&matrix))
        diagonalMatrix(dynamic_cast<const CDiagonalMatrix&>(matrix));
}