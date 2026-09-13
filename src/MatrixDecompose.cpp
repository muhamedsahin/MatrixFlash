//
// Created by muham on 12.09.2026.
//
#include "Matrix/Matrix.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

LUDecomposition Matrix::lu_decompose() const {
    if (!isSquare()) throw std::invalid_argument("Hata: LU sadece kare matrisler icin!");

    int n = this->rows;
    Matrix L = Matrix::identity(n); // Köşegeni 1 olan birim matris
    Matrix U = this->clone();       // Başlangıçta U = A
    Matrix P = Matrix::identity(n); // Permütasyon matrisi

    for (int k = 0; k < n - 1; ++k) {
        // Partial Pivoting
        int maxRow = k;
        double maxVal = std::abs(U.at(k, k));
        for (int i = k + 1; i < n; ++i) {
            if (std::abs(U.at(i, k)) > maxVal) {
                maxVal = std::abs(U.at(i, k));
                maxRow = i;
            }
        }

        if (maxVal < 1e-12) continue; // Tekil matris, atla

        if (maxRow != k) {
            // U, L ve P'nin k. satırından sonrasını takas et
            for (int j = k; j < n; ++j) std::swap(U.at(k, j), U.at(maxRow, j));
            for (int j = 0; j < k; ++j) std::swap(L.at(k, j), L.at(maxRow, j));
            for (int j = 0; j < n; ++j) std::swap(P.at(k, j), P.at(maxRow, j));
        }

        // Eliminasyon
        for (int i = k + 1; i < n; ++i) {
            double factor = U.at(i, k) / U.at(k, k);
            L.at(i, k) = factor;
            for (int j = k; j < n; ++j) {
                U.at(i, j) -= factor * U.at(k, j);
            }
        }
    }
    return {L, U, P};
}