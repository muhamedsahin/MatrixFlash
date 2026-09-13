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

    // 3. Matrisi kopyala (döngü yerine blok memcpy - daha hızlı)
    std::memcpy(LU.data(), this->data.data(), (size_t)n * n * sizeof(double));

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
    // Her sütun için ayrı ayrı çöz.
    // Performans: b/y/x tamponlarını döngü dışına çekerek
    // gereksiz n× allocation maliyetinden kurtulduk.
    // ============================================

    // Tek seferlik çalışma tamponları (her sütun için yeniden yok!)
    std::vector<double> b(n);
    std::vector<double> y(n);
    std::vector<double> x(n);

    // Her sütunu çöz
    for (int col = 0; col < n; ++col) {
        std::fill(b.begin(), b.end(), 0.0);
        b[pivot[col]] = 1.0; // Permutation uygulanmış birim vektör

        // Forward substitution: L * y = b
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            const double* lu_row = &LU[i * n];
            for (int j = 0; j < i; ++j) {
                sum += lu_row[j] * y[j];
            }
            y[i] = b[i] - sum;
        }

        // Backward substitution: U * x = y
        for (int i = n - 1; i >= 0; --i) {
            double sum = 0.0;
            const double* lu_row = &LU[i * n];
            for (int j = i + 1; j < n; ++j) {
                sum += lu_row[j] * x[j];
            }
            x[i] = (y[i] - sum) / lu_row[i];
        }

        // Sonucu sütun olarak yerleştir
        for (int i = 0; i < n; ++i) {
            result[i * n + col] = x[i];
        }
    }

    // ============================================
    // PHASE 3: Sonucu Matrix nesnesine taşı
    // result ve inv satır-major ardışık olduğu için
    // tek blok memcpy yeterlidir (en hızlı kopya).
    // ============================================
    Matrix inv(n, n);
    std::memcpy(inv.data.data(), result.data(), (size_t)n * n * sizeof(double));

    return inv;
}