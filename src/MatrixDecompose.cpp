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

    // at() sınır kontrolünü iç döngüde tekrar etmemek için ham pointer'lar
    double* Ld = L.data.data();
    double* Ud = U.data.data();
    double* Pd = P.data.data();
    const int stride = n;

    for (int k = 0; k < n - 1; ++k) {
        // Partial Pivoting
        int maxRow = k;
        double maxVal = std::abs(Ud[k * stride + k]);
        for (int i = k + 1; i < n; ++i) {
            double v = std::abs(Ud[i * stride + k]);
            if (v > maxVal) {
                maxVal = v;
                maxRow = i;
            }
        }

        if (maxVal < 1e-12) continue; // Tekil matris, atla

        if (maxRow != k) {
            // U, L ve P'nin k. satırından sonrasını takas et
            double* uk = &Ud[k * stride];
            double* um = &Ud[maxRow * stride];
            for (int j = k; j < n; ++j) std::swap(uk[j], um[j]);

            double* lk = &Ld[k * stride];
            double* lm = &Ld[maxRow * stride];
            for (int j = 0; j < k; ++j) std::swap(lk[j], lm[j]);

            double* pk = &Pd[k * stride];
            double* pm = &Pd[maxRow * stride];
            for (int j = 0; j < n; ++j) std::swap(pk[j], pm[j]);
        }

        // Eliminasyon
        double inv_piv = 1.0 / Ud[k * stride + k]; // bölme yerine çarpma
        double* uk = &Ud[k * stride];

        for (int i = k + 1; i < n; ++i) {
            double factor = Ud[i * stride + k] * inv_piv;
            Ld[i * stride + k] = factor;
            double* ui = &Ud[i * stride];
            for (int j = k; j < n; ++j) {
                ui[j] -= factor * uk[j];
            }
        }
    }
    return {L, U, P};
}