//
// Created by muham on 12.09.2026.
//

#include <vector>
#include <cmath>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include "Matrix/Matrix.hpp"

Matrix Matrix::Inverse() const {

    if (this->cols != this->rows) {
        throw std::invalid_argument("Hata: Sadece kare matrislerin tersi alinabilir!");
    }

    int n = this -> rows;
    int augCols = 2*n;

    std::vector<double> aug(n * augCols,0.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            aug[i * augCols + j] = data[i * n + j];// Kendi asıl matrixsimiz
        }
        aug[i * augCols + (n + i)] = 1.0;// Birim matrix kısımı köşegenler için
    }



    // --- GAUSS-JORDAN BAŞLANGICI ---

    // Not: Büyük matrisler için buraya #pragma omp parallel for eklenebilir
    // Ancak satır takası (swap) serial bağımlılık yarattığı için dikkatli olunmalı.
    // En güvenli ve hızlı yöntem, kritik bölümü serial, ağır hesaplamaları SIMD ile yapmaktır.
    /*
     * pivloma nedir aslında amaç şudur her sütun taranır ve daha sonrasında taradığımız her
     * bir sütundaki mutlak değeri en büyük olan değer tespit edilir ve en büyük değerin
     * bulunduğu satır en başa çıkartılır amaç şudur ilk önce 1. sütundan başlanır en büyük
     * mutlak değerli o sütunun içinde bulunan satır 1. satır ile değiştirilip kitletilir
     * böyle böyle kitleme işlemi devam eder en sonunda kitlenecek hiç bir satır kalmaz
     * kural kitlenmiş satıra dokunmak yasak. Bu işlem satırdaki sayıların patlamasını yani
     * absürt derecede büyümesini engeller peki sayılar çok büyürse ne olur bellek tüketimi
     * artar daha kötüsü kullandığımız veri tipinin hassasiyetinin dışına çıkmak bütün sonucu
     * çöp edebilir bu yüzden pivloma işlemi çok önemli bir yere sahiptir
     */

    for (int i = 0; i < n; i++) {
        // 1. KISMİ PİVOTLAMA (En büyük mutlak değer)
        int maxRow = i;
        Real maxVal = std::abs(aug[i * augCols + i]);

        // Döngüyü optimize etmek için pointer kullanımı (Opsiyonel ama hızlı)
        for (int k = i + 1; k < n; k++) {
            Real val = std::abs(aug[k * augCols + i]);
            if (val > maxVal) {
                maxVal = val;
                maxRow = k;
            }
        }

        if (maxVal < 1e-12) { // Toleransı biraz daha sıkılaştırdık (Double hassasiyeti)
            throw std::runtime_error("Hata: Matris tekil (singular), tersi alinamaz!");
        }

        // Satır Takası (Swap) - std::swap modern C++'ta çok hızlıdır
        if (maxRow != i) {
            // Tüm satırı takas et (AVX destekli kütüphaneler bunu otoptimize eder)
            Real* row_i = &aug[i * augCols];
            Real* row_max = &aug[maxRow * augCols];
            for (int j = 0; j < augCols; ++j) {
                std::swap(row_i[j], row_max[j]);
            }
        }

        // 2. NORMALİZASYON (Pivot'u 1 yap)
        Real* current_row = &aug[i * augCols];
        Real pivot = current_row[i];
        Real inv_pivot = 1.0 / pivot; // Bölme işlemi pahalıdır, çarpma yapacağız

        // Compiler'a SIMD (VEKTÖREL) işlem yapabileceğini söyle
        #pragma omp simd
        for (int j = 0; j < augCols; ++j) {
            current_row[j] *= inv_pivot;
        }

        // 3. SIFIRLAMA (Diğer tüm satırlardan çıkar)
        // Bu kısım O(N^2) olduğu için en kritik bottleneck burasıdır.
        for (int k = 0; k < n; k++) {
            if (k == i) continue;

            Real* target_row = &aug[k * augCols];
            Real factor = target_row[i];

            // Eğer factor zaten 0'sa işlem yapma (Sparse matrislerde hızlandırır)
            if (std::abs(factor) < 1e-15) continue;

            // Optimizasyon:
            // current_row[i] zaten 1.0 oldu.
            // j < i olan kısımlar current_row'da 0'dır (daha önceki adımlarda sıfırlandı).
            // Bu yüzden j döngüsüne i+1'den başlayabiliriz?
            // HAYIR, çünkü augmented matrisin SAĞ tarafı (n+c) dolu ve karışık.
            // Ancak SOL tarafta i. sütunun solu (0..i-1) zaten 0'dır.
            // Yine de basitlik ve güvenlik için tam döngü bırakıyoruz,
            // fakat compiler optimizasyonu için ipucu veriyoruz.

            #pragma omp simd
            for (int j = 0; j < augCols; ++j) {
                target_row[j] -= factor * current_row[j];
            }
        }
    }

    Matrix result(n, n);
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            result.at(r, c) = aug[r * augCols + (n + c)];
        }
    }

    return result;

}