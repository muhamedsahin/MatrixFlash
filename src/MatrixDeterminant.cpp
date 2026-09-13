//
// Created by muham on 12.09.2026.
//
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include "Matrix/Matrix.hpp"

double Matrix::determinant() const {
    // 1. KARE MATRİS KONTROLÜ
    if (this->cols != this->rows) {
        throw std::invalid_argument("Hata: Sadece kare matrislerin determinantı hesaplanabilir!");
    }

    const int n = this->rows;

    // 2. HIZLI ÇIKIŞ: 1x1 matris
    if (n == 1) {
        return this->data[0];
    }

    // 3. HIZLI ÇIKIŞ: 2x2 matris (Formül ile direkt hesapla)
    if (n == 2) {
        return (this->data[0] * this->data[3]) - (this->data[1] * this->data[2]);
    }

    // 4. HIZLI ÇIKIŞ: 3x3 matris (Sarrus kuralı - çok hızlı)
    if (n == 3) {
        const double* d = this->data.data();
        return d[0] * (d[4] * d[8] - d[5] * d[7])
             - d[1] * (d[3] * d[8] - d[5] * d[6])
             + d[2] * (d[3] * d[7] - d[4] * d[6]);
    }

    // 5. BÜYÜK MATRİSLER İÇİN: LU DECOMPOSITION
    std::vector<double> LU(n * n);
    int sign = 1; // Determinant işareti (satır takası sayısı)

    // Matrisi kopyala
    for (int i = 0; i < n * n; ++i) {
        LU[i] = this->data[i];
    }

    // LU Ayrıştırması (Gaussian Elimination with Partial Pivoting)
    for (int k = 0; k < n; ++k) {
        // Partial Pivoting - en büyük elemanı bul
        double maxVal = 0.0;
        int maxRow = k;

        for (int i = k; i < n; ++i) {
            double val = std::abs(LU[i * n + k]);
            if (val > maxVal) {
                maxVal = val;
                maxRow = i;
            }
        }

        // Matris tekil (singular) - determinant 0
        if (maxVal < 1e-12) {
            return 0.0; // Erken çıkış, zaman tasarrufu!
        }

        // Satır takası gerekli mi?
        if (maxRow != k) {
            sign = -sign; // İşareti değiştir
            double* row_k = &LU[k * n];
            double* row_max = &LU[maxRow * n];

            for (int j = 0; j < n; ++j) {
                std::swap(row_k[j], row_max[j]);
            }
        }

        // Eliminasyon - altındaki satırları sıfırla
        double pivot = LU[k * n + k];
        double inv_pivot = 1.0 / pivot; // Bölme yerine çarpma (HIZ!)

        for (int i = k + 1; i < n; ++i) {
            double factor = LU[i * n + k] * inv_pivot;
            LU[i * n + k] = factor; // L matrisini sakla

            // U matrisini güncelle
            for (int j = k + 1; j < n; ++j) {
                LU[i * n + j] -= factor * LU[k * n + j];
            }
        }
    }

    // 6. DETERMİNANT HESAPLA
    // det(A) = sign × (U matrisinin köşegen elemanlarının çarpımı)
    double det = static_cast<double>(sign);

    for (int i = 0; i < n; ++i) {
        det *= LU[i * n + i]; // U'nun köşegeni
    }

    return det;
}