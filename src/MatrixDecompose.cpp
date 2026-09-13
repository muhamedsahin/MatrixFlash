//
// Created by muham on 12.09.2026.
//
#include "Matrix/Matrix.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

LUDecomposition Matrix::lu_decompose() const {
    if (!isSquare()) throw std::invalid_argument("Hata: LU sadece kare matrisler icin!");

    int n = this->rows;

    // Ortak LU çekirdeği (Inverse() ile aynı implementasyonu paylaşır)
    std::vector<double> LU;
    std::vector<int> pivot;
    if (!Matrix::luDecomposeCore(this->data, n, LU, pivot)) {
        throw std::runtime_error("Hata: Matris tekil (singular), LU ayrıştırılamaz!");
    }

    // Combined LU tamponundan L, U ve P'yi kur
    Matrix L = Matrix::zeros(n, n);   // birim köşegen + alt çarpanlar
    Matrix U = Matrix::zeros(n, n);   // üst üçgen (köşegen dahil)
    Matrix P = Matrix::zeros(n, n);   // permütasyon matrisi

    double* Ld = L.data.data();
    double* Ud = U.data.data();
    double* Pd = P.data.data();

    for (int i = 0; i < n; ++i) {
        const double* lu_row = &LU[(size_t)i * n];
        double* Lrow = &Ld[(size_t)i * n];
        double* Urow = &Ud[(size_t)i * n];

        for (int j = 0; j < n; ++j) {
            if (j < i)      { Lrow[j] = lu_row[j]; Urow[j] = 0.0; } // L: alt
            else if (j > i) { Lrow[j] = 0.0;        Urow[j] = lu_row[j]; } // U: üst
            else            { Lrow[j] = 1.0;        Urow[j] = lu_row[j]; } // köşegen (L=1, U=pivot)
        }
    }

    // P: pivot takas sırasını yansıtır -> P[i][pivot[i]] = 1
    for (int i = 0; i < n; ++i) {
        Pd[(size_t)i * n + pivot[i]] = 1.0;
    }

    return {L, U, P};
}