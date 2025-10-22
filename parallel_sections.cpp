#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <omp.h>
#include <utility>
#include "omp_for.h"

#define NUM_THREADS 2

void compute_range(float** A, float* B, float* C, int start, int end, int M) {
    for (int i = start; i < end; ++i) {
        C[i] = 0.0f;
        for (int j = 0; j < M; ++j) {
            C[i] += A[i][j] * B[j];
        }
    }
}

void matrixVectorMultiply2(float** A, float* B, float* C, int N, int M) {
#pragma omp parallel sections
    {
#pragma omp section
        compute_range(A, B, C, 0, N/2, M);

#pragma omp section
        compute_range(A, B, C, N/2, N, M);
    }
}

void matrixVectorMultiply4(float** A, float* B, float* C, int N, int M) {
#pragma omp parallel sections
    {
#pragma omp section
        compute_range(A, B, C, 0, N/4, M);

#pragma omp section
        compute_range(A, B, C, N/4, N/2, M);

#pragma omp section
        compute_range(A, B, C, N/2, 3*N/ 4, M);

#pragma omp section
        compute_range(A, B, C, 3*N/4, N, M);
    }
}

void matrixVectorMultiply8(float** A, float* B, float* C, int N, int M) {
#pragma omp parallel sections
    {
#pragma omp section
        compute_range(A, B, C, 0, N/8, M);

#pragma omp section
        compute_range(A, B, C, N/8, N/4, M);

#pragma omp section
        compute_range(A, B, C, N/4, 3*N/8, M);

#pragma omp section
        compute_range(A, B, C, 3*N/ 8, N/2, M);

#pragma omp section
        compute_range(A, B, C, N/2, 5*N/8, M);

#pragma omp section
        compute_range(A, B, C, 5*N/8, 3*N/4, M);

#pragma omp section
        compute_range(A, B, C, 3*N/4, 7*N/8, M);

#pragma omp section
        compute_range(A, B, C, 7*N/8, N, M);
    }
}

void matrixVectorMultiply16(float** A, float* B, float* C, int N, int M) {
#pragma omp parallel sections
    {
#pragma omp section
        compute_range(A, B, C, 0, N / 16, M);

#pragma omp section
        compute_range(A, B, C, N/16, N/8, M);

#pragma omp section
        compute_range(A, B, C, N/16, 3*N/16, M);

#pragma omp section
        compute_range(A, B, C, 3*N/16, N/4, M);

#pragma omp section
        compute_range(A, B, C, N/4, 5*N/16, M);

#pragma omp section
        compute_range(A, B, C, 5*N/16, 3*N/8, M);

#pragma omp section
        compute_range(A, B, C, 3*N/8, 7*N/16, M);

#pragma omp section
        compute_range(A, B, C, 7*N/16, N/2, M);

#pragma omp section
        compute_range(A, B, C, N/2, 9*N/16, M);

#pragma omp section
        compute_range(A, B, C, 9*N/16, 5*N/8, M);

#pragma omp section
        compute_range(A, B, C, 5*N/8, 11*N/16, M);

#pragma omp section
        compute_range(A, B, C, 11*N/16, 3*N/4, M);

#pragma omp section
        compute_range(A, B, C, 3*N/4, 13*N/16, M);

#pragma omp section
        compute_range(A, B, C, 13*N/16, 7*N/8, M);

#pragma omp section
        compute_range(A, B, C, 7*N/8, 15*N/16, M);

#pragma omp section
        compute_range(A, B, C, 15*N/16, N, M);
    }
}

std::pair<double, std::chrono::milliseconds> mult_matrix_parallel_manual(int N, int M) {
    std::vector<std::vector<float>> A(N, std::vector<float>(M));
    std::vector<float> B(M);
    std::vector<float> C(N, 0.0f);

    std::mt19937 gen(42 + rand() % 100); // Different seed for each iteration
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // Convert std::vector to raw pointers for compatibility with matrixVectorMultiply
    float** A_ptr = new float* [N];
    for (int i = 0; i < N; ++i) A_ptr[i] = A[i].data();
    float* B_ptr = B.data();
    float* C_ptr = C.data();

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
    //matrixVectorMultiply2(A_ptr, B_ptr, C_ptr, N, M);
    //matrixVectorMultiply4(A_ptr, B_ptr, C_ptr, N, M);
    matrixVectorMultiply8(A_ptr, B_ptr, C_ptr, N, M);
    //matrixVectorMultiply16(A_ptr, B_ptr, C_ptr, N, M);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration_seconds = end - start;
    std::chrono::milliseconds duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration_seconds);

    // Compute sum of result vector for verification
    float sum = 0.0f;
    for (float val : C) sum += val;

    delete[] A_ptr;

    return std::make_pair(static_cast<double>(sum), duration_ms);
}