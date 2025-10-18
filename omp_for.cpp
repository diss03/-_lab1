#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>
#include "omp_for.h"

void parallel_for_multiplication(int N, int M) {
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

    // ѕараллельное умножение матрицы на вектор
#pragma omp parallel for
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            C[i] += A[i][j] * B[j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "OMP for time: " << duration.count() << " seconds" << std::endl;
    std::cout << "Number of threads used: " << omp_get_max_threads() << std::endl;

    float sum = 0.0f;
    for (float val : C) sum += val;
    std::cout << "Sum of C: " << sum << std::endl;
}