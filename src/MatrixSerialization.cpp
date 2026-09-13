//
// Created by muham on 12.09.2026.
//
#include "Matrix/Matrix.hpp"
#include <fstream>

// DOSYAYA KAYDET (Ultra Hızlı Binary)
void Matrix::saveBinary(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Dosya acilamadi: " + filename);

    // 1. Başlık (Header) yaz: Satır ve Sütun sayısı
    file.write(reinterpret_cast<const char*>(&rows), sizeof(int));
    file.write(reinterpret_cast<const char*>(&cols), sizeof(int));

    // 2. Ham veriyi (data vector) doğrudan diske dök
    // reinterpret_cast, double dizisini byte dizisi gibi davranmaya zorlar
    file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(double));

    file.close();
}

// DOSYADAN OKU (Ultra Hızlı Binary)
Matrix Matrix::loadBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) throw std::runtime_error("Dosya okunamadi: " + filename);

    int r, c;
    // 1. Başlığı oku
    if (!file.read(reinterpret_cast<char*>(&r), sizeof(int)) ||
        !file.read(reinterpret_cast<char*>(&c), sizeof(int))) {
        throw std::runtime_error("Matris dosyasi basligi bozuk: " + filename);
    }
    if (r < 0 || c < 0) {
        throw std::runtime_error("Matris dosyasinda gecersiz boyut: " + filename);
    }

    // 2. Yeni matrisi oluştur
    Matrix result(r, c);

    // 3. Ham veriyi doğrudan vector'ün içine oku
    if (!file.read(reinterpret_cast<char*>(result.data.data()),
                   static_cast<std::streamsize>(result.data.size() * sizeof(double)))) {
        throw std::runtime_error("Matris verisi eksik veya bozuk: " + filename);
    }

    file.close();
    return result;
}
