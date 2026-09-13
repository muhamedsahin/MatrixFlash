//
// Created by muham on 9.09.2026.
//

#pragma once

#include <iostream>
#include <string>

class Console {
public:
    // 1. DURUM: Hiç parametre kalmadıysa (Sadece alt satıra geçer)
    // Bu, özyinelemenin (recursion) bitiş noktasıdır.
    void log() {
        std::cout << std::endl;
    }

    // 2. DURUM: En az bir parametre varsa (SİHİRLİ KISIM)
    // T: İlk parametrenin türü (int, string, float, bool vs. olabilir)
    // Args...: Kalan tüm parametrelerin paketi
    template<typename T, typename... Args>
    void log(T first, Args... args) {
        // Eğer gelen değer bool ise "true"/"false" yazsın, değilse normal yazsın
        if constexpr (std::is_same_v<T, bool>) {
            std::cout << (first ? "true" : "false") << " ";
        } else {
            std::cout << first << " ";
        }

        // Fonksiyonu, KALAN parametrelerle tekrar çağır!
        log(args...);
    }

    // --- AYNI MANTIK print İÇİN DE GEÇERLİDİR ---

    void print() {
        // print için bitiş noktası (boş bırak)
    }

    template<typename T, typename... Args>
    void print(T first, Args... args) {
        if constexpr (std::is_same_v<T, bool>) {
            std::cout << (first ? "true" : "false") << " ";
        } else {
            std::cout << first << " ";
        }
        print(args...);
    }
};