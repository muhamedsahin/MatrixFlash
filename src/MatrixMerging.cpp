//
// Created by muham on 12.09.2026.
//
#include <vector>
#include <cstring> // memcpy için
#include <stdexcept>
#include <algorithm>
#include "Matrix/Matrix.hpp"

Matrix Matrix::slice(int rowStart, int rowEnd, int colStart, int colEnd) const {
    // Gerekli Matrix kontrol işlemlerini yapıyoruz.
    if (rowStart < 0 || rowEnd > this->rows || rowStart >= rowEnd) {
        throw std::out_of_range("Hata: Gecersiz satir araligi!");
    }
    if (colStart < 0 || colEnd > this->cols || colStart >= colEnd) {
        throw std::out_of_range("Hata: Gecersiz sutun araligi!");
    }

    // Yeni oluşturulacak matrix col row bilgileri
    const int newRows = rowEnd - rowStart;
    const int newCols = colEnd - colStart;

    // Result adında yeni bir matix nesnesi oluşturuyoruz
    Matrix result(newRows, newCols);

    // 3. OPTİMİZE KOPYALAMA
    // DURUM A: Tüm sütunları alıyorsak (colStart=0, colEnd=cols)
    // → Sürekli bellek blokları, memcpy kullan (EN HIZLI!)
    if (colStart == 0 && colEnd == this->cols) {
        const size_t rowBytes = newCols * sizeof(double);

        for (int i = 0; i < newRows; ++i) {
            const double* src = &this->data[(rowStart + i) * this->cols];
            double* dst = &result.data[i * newCols];
            std::memcpy(dst, src, rowBytes);
        }
    }
    // DURUM B: Sütunların bir kısmını alıyorsak
    // → Kesintili bellek, satır satır kopyala
    else {
        const size_t copyBytes = newCols * sizeof(double);

        for (int i = 0; i < newRows; ++i) {
            const double* src = &this->data[(rowStart + i) * this->cols + colStart];
            double* dst = &result.data[i * newCols];
            std::memcpy(dst, src, copyBytes);
        }
    }

    return result;
}

Matrix Matrix::concatVertical(const Matrix& other) const {
    // 1. BOYUT KONTROLÜ
    if (this->cols != other.cols) {
        throw std::invalid_argument("Hata: Dikey birlestirme icin sutun sayilari esit olmali!");
    }

    const int totalRows = this->rows + other.rows;
    const int totalCols = this->cols;

    // 2. SONUÇ MATRİSİ OLUŞTUR
    Matrix result(totalRows, totalCols);

    // 3. OPTİMİZE KOPYALAMA
    const size_t rowBytes = totalCols * sizeof(double);

    // İlk matrisi kopyala
    for (int i = 0; i < this->rows; ++i) {
        const double* src = &this->data[i * this->cols];
        double* dst = &result.data[i * totalCols];
        std::memcpy(dst, src, rowBytes);
    }

    // İkinci matrisi kopyala
    for (int i = 0; i < other.rows; ++i) {
        const double* src = &other.data[i * other.cols];
        double* dst = &result.data[(this->rows + i) * totalCols];
        std::memcpy(dst, src, rowBytes);
    }

    return result;
}

Matrix Matrix::concatHorizontal(const Matrix& other) const {
    // 1. BOYUT KONTROLÜ
    if (this->rows != other.rows) {
        throw std::invalid_argument("Hata: Yatay birlestirme icin satir sayilari esit olmali!");
    }

    const int totalRows = this->rows;
    const int totalCols = this->cols + other.cols;

    // 2. SONUÇ MATRİSİ OLUŞTUR
    Matrix result(totalRows, totalCols);

    // 3. OPTİMİZE KOPYALAMA
    const size_t bytes1 = this->cols * sizeof(double);
    const size_t bytes2 = other.cols * sizeof(double);

    for (int i = 0; i < totalRows; ++i) {
        // İlk matrisin i. satırını kopyala
        const double* src1 = &this->data[i * this->cols];
        double* dst1 = &result.data[i * totalCols];
        std::memcpy(dst1, src1, bytes1);

        // İkinci matrisin i. satırını kopyala
        const double* src2 = &other.data[i * other.cols];
        double* dst2 = &result.data[i * totalCols + this->cols];
        std::memcpy(dst2, src2, bytes2);
    }

    return result;
}

Matrix Matrix::concatenate(const std::vector<const Matrix*>& matrices, int axis) {
    if (matrices.empty()) {
        throw std::invalid_argument("Hata: Bos matris listesi!");
    }

    // 1. TOPLAM BOYUT HESAPLA
    int totalRows = 0, totalCols = 0;

    if (axis == 0) { // Dikey
        totalCols = matrices[0]->cols;
        for (const auto* m : matrices) {
            if (m->cols != totalCols) {
                throw std::invalid_argument("Hata: Dikey birlestirme icin sutun sayilari esit olmali!");
            }
            totalRows += m->rows;
        }
    } else if (axis == 1) { // Yatay
        totalRows = matrices[0]->rows;
        for (const auto* m : matrices) {
            if (m->rows != totalRows) {
                throw std::invalid_argument("Hata: Yatay birlestirme icin satir sayilari esit olmali!");
            }
            totalCols += m->cols;
        }
    } else {
        throw std::invalid_argument("Hata: axis sadece 0 (dikey) veya 1 (yatay) olabilir!");
    }

    // 2. SONUÇ MATRİSİ OLUŞTUR
    Matrix result(totalRows, totalCols);

    // 3. OPTİMİZE KOPYALAMA
    if (axis == 0) { // Dikey birleştirme
        int currentRow = 0;
        for (const auto* m : matrices) {
            const size_t rowBytes = m->cols * sizeof(double);

            for (int i = 0; i < m->rows; ++i) {
                const double* src = &m->data[i * m->cols];
                double* dst = &result.data[(currentRow + i) * totalCols];
                std::memcpy(dst, src, rowBytes);
            }
            currentRow += m->rows;
        }
    } else { // Yatay birleştirme
        for (int i = 0; i < totalRows; ++i) {
            int currentCol = 0;
            for (const auto* m : matrices) {
                const size_t bytes = m->cols * sizeof(double);
                const double* src = &m->data[i * m->cols];
                double* dst = &result.data[i * totalCols + currentCol];
                std::memcpy(dst, src, bytes);
                currentCol += m->cols;
            }
        }
    }

    return result;
}