#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>
#include <utility>
#include "omp_for.h"

#define NUM_THREADS 2

template<typename T>
void matrixVectorMultiply(T** A, T* B, T* C, int N, int M) {
#pragma omp section
    {
        for (int i = 0; i < N / 2; ++i) {
            C[i] = 0.0;
            for (int j = 0; j < M; ++j) {
                C[i] += A[i][j] * B[j];
            }
        }
    }
}


std::pair<double, std::chrono::milliseconds> mult_matrix_parallel_manual(int N, int M) {
    std::vector<std::vector<float>> A(N, std::vector<float>(M));
    std::vector<float> B(M);
    std::vector<float> C(N, 0.0f);

    std::mt19937 gen(42 + rand() % 100); // Different seed for each iteration
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Initialize matrix A and vector B
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            A[i][j] = dist(gen);
        }
    }
    for (int j = 0; j < M; ++j) {
        B[j] = dist(gen);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Parallel matrix-vector multiplication using sections
#pragma omp parallel sections
    {
#pragma omp section
        {
            for (int i = 0; i < N / 2; ++i) {
                C[i] = 0.0f;
                for (int j = 0; j < M; ++j) {
                    C[i] += A[i][j] * B[j];
                }
            }
        }
#pragma omp section
        {
            for (int i = N / 2; i < N; ++i) {
                C[i] = 0.0f;
                for (int j = 0; j < M; ++j) {
                    C[i] += A[i][j] * B[j];
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_seconds = end - start;
    std::chrono::milliseconds duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_seconds);

    // Compute sum of result vector for verification
    float sum = 0.0f;
    for (float val : C) sum += val;

    return std::make_pair(static_cast<double>(sum), duration_ms);
}