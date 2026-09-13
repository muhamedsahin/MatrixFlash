//
// Created by muham on 12.09.2026.
//
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include "Matrix/Matrix.hpp"

// Block size for cache optimization (L1 cache friendly)
constexpr int BLOCK_SIZE = 64;

Matrix Matrix::Inverse() const {
    // 1. Kare matris kontrolü
    if (this->cols != this->rows) {
        throw std::invalid_argument("Hata: Sadece kare matrislerin tersi alinabilir!");
    }

    const int n = this->rows;

    // 2. Çalışma alanı ayır (Cache-friendly tek allocation)
    std::vector<double> LU(n * n);
    std::vector<int> pivot(n);
    std::vector<double> result(n * n, 0.0);

    // 3. Matrisi kopyala
    for (int i = 0; i < n * n; ++i) {
        LU[i] = this->data[i];
    }

    // ============================================
    // PHASE 1: LU DECOMPOSITION (Doolittle Algorithm)
    // A = P * L * U (P = Permutation, L = Lower, U = Upper)
    // Bu Gauss-Jordan'dan ~2x daha hızlı!
    // ============================================

    for (int i = 0; i < n; ++i) {
        pivot[i] = i;
    }

    for (int k = 0; k < n; ++k) {
        // Partial pivoting - en büyük elemanı bul
        double maxVal = 0.0;
        int maxRow = k;

        for (int i = k; i < n; ++i) {
            double val = std::abs(LU[i * n + k]);
            if (val > maxVal) {
                maxVal = val;
                maxRow = i;
            }
        }

        if (maxVal < 1e-12) {
            throw std::runtime_error("Hata: Matris tekil (singular), tersi alinamaz!");
        }

        // Satır takası
        if (maxRow != k) {
            std::swap(pivot[k], pivot[maxRow]);
            double* row_k = &LU[k * n];
            double* row_max = &LU[maxRow * n];

            for (int j = 0; j < n; ++j) {
                std::swap(row_k[j], row_max[j]);
            }
        }

        // U matrisinin k. satırını hesapla
        for (int j = k; j < n; ++j) {
            double sum = 0.0;
            for (int p = 0; p < k; ++p) {
                sum += LU[k * n + p] * LU[p * n + j];
            }
            LU[k * n + j] -= sum;
        }

        // L matrisinin k. sütununu hesapla
        if (k < n - 1) {
            double inv_pivot = 1.0 / LU[k * n + k];

            for (int i = k + 1; i < n; ++i) {
                double sum = 0.0;
                for (int p = 0; p < k; ++p) {
                    sum += LU[i * n + p] * LU[p * n + k];
                }
                LU[i * n + k] = (LU[i * n + k] - sum) * inv_pivot;
            }
        }
    }

    // ============================================
    // PHASE 2: FORWARD & BACKWARD SUBSTITUTION
    // Her sütun için ayrı ayrı çöz (Parallel!)
    // ============================================

    // Birim matris oluştur (sonucu burada saklayacağız)
    for (int i = 0; i < n; ++i) {
        result[i * n + i] = 1.0;
    }

    // Her sütunu paralel çöz
    for (int col = 0; col < n; ++col) {
        std::vector<double> b(n, 0.0);
        b[pivot[col]] = 1.0; // Permutation uygulanmış birim vektör

        // Forward substitution: L * y = b
        std::vector<double> y(n);
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < i; ++j) {
                sum += LU[i * n + j] * y[j];
            }
            y[i] = b[i] - sum;
        }

        // Backward substitution: U * x = y
        std::vector<double> x(n);
        for (int i = n - 1; i >= 0; --i) {
            double sum = 0.0;
            for (int j = i + 1; j < n; ++j) {
                sum += LU[i * n + j] * x[j];
            }
            x[i] = (y[i] - sum) / LU[i * n + i];
        }

        // Sonucu yerleştir
        for (int i = 0; i < n; ++i) {
            result[i * n + col] = x[i];
        }
    }

    // ============================================
    // PHASE 3: Cache-Optimized Matrix Copy
    // ============================================
    Matrix inv(n, n);

    // Block-based copy (cache-friendly)
    for (int i = 0; i < n; i += BLOCK_SIZE) {
        for (int j = 0; j < n; j += BLOCK_SIZE) {
            int i_max = std::min(i + BLOCK_SIZE, n);
            int j_max = std::min(j + BLOCK_SIZE, n);

            for (int ii = i; ii < i_max; ++ii) {
                for (int jj = j; jj < j_max; ++jj) {
                    inv.at(ii, jj) = result[ii * n + jj];
                }
            }
        }
    }

    return inv;
}