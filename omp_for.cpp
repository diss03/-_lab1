#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>
#include <utility>
#include "omp_for.h"
#define NUM_THREADS 12

std::pair<double, std::chrono::milliseconds> parallel_for_multiplication(int N, int M) {
    std::vector<std::vector<float>> A(N, std::vector<float>(M));
    std::vector<float> B(M);
    std::vector<float> C(N, 0.0f);

    std::mt19937 gen(42 + rand() % 100); // Разный seed для каждой итерации
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Инициализация и вывод исходных данных
    //std::cout << "Parallel Multiplication - Initial Data:" << std::endl;
    //std::cout << "Matrix A (" << N << "x" << M << "):" << std::endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            A[i][j] = dist(gen);
            //std::cout << A[i][j] << "\t";
        }
        //std::cout << std::endl;
    }
    //std::cout << "Vector B (" << M << "):" << std::endl;
    for (int j = 0; j < M; ++j) {
        B[j] = dist(gen);
        //std::cout << B[j] << "\t";
    }
    //std::cout << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    // Параллельное умножение
#pragma omp parallel for num_threads(NUM_THREADS)
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            C[i] += A[i][j] * B[j];
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_seconds = end - start;
    std::chrono::milliseconds duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_seconds);

    // Вывод результата
    //std::cout << "Result Vector C (" << N << "):" << std::endl;
    for (int i = 0; i < N; ++i) {
        //std::cout << C[i] << "\t";
    }
    //std::cout << std::endl;

    float sum = 0.0f;
    for (float val : C) sum += val;

    return std::make_pair(static_cast<double>(sum), duration_ms);
}