#include "evalutaion.hpp"
using namespace std;

int main() {
    mcoord_t matrix;
    matrix.loadFromMatrixMarketFile("test data\\matrix_430_430.mtx");
    CEvaluation::storageEfficiency(matrix);
    CEvaluation::storageEfficiency(mdiag_t(matrix));
    CEvaluation::storageEfficiency(msdiag_t32(matrix));
    CEvaluation::storageEfficiency(msdiag_t64(matrix));
    return 0;
}