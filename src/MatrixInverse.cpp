//
// Created by muham on 12.09.2026.
//
#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include "Matrix/Matrix.hpp"

// =========================================================
// ORTAK LU ÇEKİRDEĞİ
// ---------------------------------------------------------
// lu_decompose() ile Inverse() bu tek implementasyonu paylaşır
// (kod tekrarını önler). A = P*L*U ayrıştırmasını tek bir LU
// tamponunda yapar: alt üçgen L çarpanlarını, üst (köşegen dahil)
// U'yu tutar. pivot[] ise uygulanan satır takaslarını kaydeder.
// Dönüş değeri false = matris tekil (singular).
// =========================================================
bool Matrix::luDecomposeCore(const std::vector<double>& src, int n,
                             std::vector<double>& LU, std::vector<int>& pivot) {
    LU.resize((size_t)n * n);
    pivot.resize(n);

    // Kaynak matrisi çalışma tamponuna al (blok memcpy - cache dostu)
    std::memcpy(LU.data(), src.data(), (size_t)n * n * sizeof(double));

    for (int i = 0; i < n; ++i) {
        pivot[i] = i;
    }

    bool ok = true;

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
            ok = false; // Teğil: işareti, elimasyona devam etme
            continue;
        }

        // Satır takası
        if (maxRow != k) {
            std::swap(pivot[k], pivot[maxRow]);
            double* row_k = &LU[(size_t)k * n];
            double* row_max = &LU[(size_t)maxRow * n];

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

    return ok;
}

Matrix Matrix::Inverse() const {
    // 1. Kare matris kontrolü
    if (this->cols != this->rows) {
        throw std::invalid_argument("Hata: Sadece kare matrislerin tersi alinabilir!");
    }

    const int n = this->rows;

    // 2. Çalışma alanı + ortak LU ayrıştırması (lu_decompose ile paylaşılır)
    std::vector<double> LU;
    std::vector<int> pivot;
    std::vector<double> result((size_t)n * n, 0.0);

    if (!luDecomposeCore(this->data, n, LU, pivot)) {
        throw std::runtime_error("Hata: Matris tekil (singular), tersi alinamaz!");
    }

    // ============================================
    // PHASE 2: FORWARD & BACKWARD SUBSTITUTION
    // Her sütun için ayrı ayrı çöz.
    // Performans: b/y/x tamponları döngü öncesi bir kez ayrılır.
    // ============================================
    std::vector<double> b(n);
    std::vector<double> y(n);
    std::vector<double> x(n);

    for (int col = 0; col < n; ++col) {
        std::fill(b.begin(), b.end(), 0.0);
        b[pivot[col]] = 1.0; // Permutation uygulanmış birim vektör

        // Forward substitution: L * y = b
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            const double* lu_row = &LU[(size_t)i * n];
            for (int j = 0; j < i; ++j) {
                sum += lu_row[j] * y[j];
            }
            y[i] = b[i] - sum;
        }

        // Backward substitution: U * x = y
        for (int i = n - 1; i >= 0; --i) {
            double sum = 0.0;
            const double* lu_row = &LU[(size_t)i * n];
            for (int j = i + 1; j < n; ++j) {
                sum += lu_row[j] * x[j];
            }
            x[i] = (y[i] - sum) / lu_row[i];
        }

        // Sonucu sütun olarak yerleştir
        for (int i = 0; i < n; ++i) {
            result[(size_t)i * n + col] = x[i];
        }
    }

    // ============================================
    // PHASE 3: Sonucu Matrix nesnesine taşı (tek blok memcpy)
    // ============================================
    Matrix inv(n, n);
    std::memcpy(inv.data.data(), result.data(), (size_t)n * n * sizeof(double));

    return inv;
}