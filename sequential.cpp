#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <utility>  // Для std::pair
#include "sequential.h"

std::pair<double, std::chrono::milliseconds> sequential_multiplication(int N, int M) {
    std::vector<std::vector<float>> A(N, std::vector<float>(M));
    std::vector<float> B(M);
    std::vector<float> C(N, 0.0f);

    std::mt19937 gen(42); // Фиксированный seed для воспроизводимости
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Инициализация матрицы и вектора случайными значениями
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            A[i][j] = dist(gen);
        }
    }
    for (int j = 0; j < M; ++j) {
        B[j] = dist(gen);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Последовательное умножение матрицы на вектор
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            C[i] += A[i][j] * B[j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_seconds = end - start;
    std::chrono::milliseconds duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_seconds);

    float sum = 0.0f;
    for (float val : C) sum += val;

    // Возвращаем сумму и время в миллисекундах
    return std::make_pair(static_cast<double>(sum), duration_ms);
}