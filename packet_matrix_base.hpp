#ifndef SPARSEMATRIX_PACKET_MATRIX_BASE_HPP
#define SPARSEMATRIX_PACKET_MATRIX_BASE_HPP
#include <cinttypes>
#include <vector>

//typedef unsigned int size_t;

class IPacketMatrixBase {
public:
    virtual size_t getNZ() const = 0;
    virtual size_t getN() const = 0;
    virtual uint64_t getMatrixSize() const = 0;
};

#endif //SPARSEMATRIX_PACKET_MATRIX_BASE_HPP
