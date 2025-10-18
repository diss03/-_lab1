#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include "sequential.h"

void sequential_multiplication(int N, int M) {
    std::vector<std::vector<float>> A(N, std::vector<float>(M));
    std::vector<float> B(M);
    std::vector<float> C(N, 0.0f);

    std::mt19937 gen(42); // ‘иксированный seed дл€ воспроизводимости
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // »нициализаци€ матрицы и вектора случайными значени€ми
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            A[i][j] = dist(gen);
        }
    }
    for (int j = 0; j < M; ++j) {
        B[j] = dist(gen);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // ѕоследовательное умножение матрицы на вектор
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            C[i] += A[i][j] * B[j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Sequential time: " << duration.count() << " seconds" << std::endl;

    float sum = 0.0f;
    for (float val : C) sum += val;
    std::cout << "Sum of C: " << sum << std::endl;
}