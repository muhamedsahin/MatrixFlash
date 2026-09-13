# MatrixFlash

MatrixFlash, yapay zeka çalışmaları için kullanılan küçük ve bağımsız bir C++17 matris kütüphanesidir.

## Proje ve geliştirici

Bu proje, Bursa Teknik Üniversitesi'nde öğrenim gören 1. sınıf öğrencisi **Muhammed Fatih Şahin** tarafından tasarlanmış, yazılmış ve geliştirilmektedir.

Bu depodaki MatrixFlash kaynak kodunun tamamı Muhammed Fatih Şahin'e aittir ve proje onun tarafından geliştirilmektedir. Proje eğitim, araştırma ve kişisel yazılım geliştirme amacıyla hazırlanmıştır.

MatrixFlash tamamen ücretsizdir ve MIT Lisansı ile yayımlanmaktadır. Lisansın tam metni için [LICENSE](LICENSE) dosyasına bakabilirsiniz.

`src/main.cpp` örnek uygulamadır. Bu dosyayı tek başına `gcc` ile derlemeyin; kütüphane birden fazla kaynak dosyasından oluştuğu için CMake hedefini kullanın.

[![Documentation](https://img.shields.io/badge/docs-live-brightgreen)](https://muhamedsahin.github.io/MatrixFlash/)

## Klasör yapısı

```text
include/Matrix/Matrix.hpp  Public API
src/                       Kütüphane implementasyonları
tests/MatrixTests.cpp      CTest testleri
examples/                   Çalıştırılabilir kullanım örnekleri
src/main.cpp               Manuel deneme uygulaması
```

Kaynak dosyaları sorumluluklarına göre ayrılmıştır:

- `Matrix.cpp`: yaşam döngüsü, erişim, kopyalama ve boyut bilgileri
- `MatrixArithmetic.cpp`: toplama, çıkarma, matris, element bazlı ve skaler çarpma
- `MatrixActivation.cpp`: softmax ve ReLU işlemleri
- `MatrixTransforms.cpp`: identity, ones, zeros, flatten, transpose ve randomize
- `MatrixStatistics.cpp`: ortalama, norm, trace ve negate
- `MatrixInverse.cpp`, `MatrixDeterminant.cpp`: ileri seviye kare matris işlemleri
- `MatrixMerging.cpp`: slice ve concat işlemleri
- `MatrixDecompose.cpp`: LU ayrıştırması
- `MatrixEigenvalue.cpp`: power iteration
- `MatrixBroadcasting.cpp`: broadcasting işlemleri
- `MatrixSerialization.cpp`: binary kaydetme/yükleme

## Derleme ve çalıştırma

`MartixFlash` klasöründe:

```powershell
cmake -S . -B build -G Ninja
cmake --build build --parallel
./build/MatrixApp.exe
```

Windows yolunu kabuklar arasında taşımak için şu biçim de kullanılabilir:

```powershell
& "./build/MatrixApp.exe"
```

CLion kullanıyorsanız `MatrixApp` hedefini seçip çalıştırın. VS Code’daki tek dosya çalıştırma eklentisini kullanmayın; o yalnızca aktif `.cpp` dosyasını derler ve diğer Matrix kaynaklarını linklemez.

## Testleri çalıştırma

```powershell
cmake --build build --target MatrixTests --parallel
ctest --test-dir build --output-on-failure
```

## Örnekleri çalıştırma

Örnek hedefleri şunlardır:

- `MatrixExampleBasic`: temel oluşturma, çarpma, ones ve slice
- `MatrixExampleAiPipeline`: broadcasting ve softmax ile basit yapay zeka akışı
- `MatrixExampleAdvanced`: determinant, inverse, LU ve eigenvalue
- `MatrixExampleStorage`: concat, slice ve binary serialization

Örneğin:

```powershell
cmake --build build --target MatrixExampleAiPipeline --parallel
./build/examples/MatrixExampleAiPipeline.exe
```

Tüm örnekleri tek seferde derlemek için:

```powershell
cmake --build build --parallel
```

Testler constructor/access, copy-move, tüm aritmetik işlemler, dönüşümler, istatistikler, softmax, slice/concat, determinant, inverse, LU, eigenvalue, broadcasting, serialization ve `print` akışlarını kontrol eder.

Yeni bir özellik eklerken:

1. Public bildirimi `include/Matrix/Matrix.hpp` içine ekle.
2. Uygulamayı ilgili `src/Matrix*.cpp` dosyasına koy.
3. Davranış testini `tests/MatrixTests.cpp` içine ekle.
4. Build ve CTest çalıştır.

## Dokümantasyon sitesi

Kütüphanenin kullanımını, matris matematiğini, API referansını ve etkileşimli örnekleri görmek için:

- [MatrixFlash dokümantasyonunu aç](docs/index.html)
- Siteyi doğrudan tarayıcıda açabilir veya `docs` klasörünü herhangi bir statik web sunucusuyla yayınlayabilirsiniz.

## Lisans

Copyright (c) 2026 Muhammed Fatih Şahin

Bu proje MIT Lisansı altında ücretsiz olarak kullanılabilir, kopyalanabilir, değiştirilebilir ve dağıtılabilir.
