#pragma once
#ifndef PAR_SEC_H
#define PAR_SEC_H

#include <vector>

// ќбъ€вление функции параллельного умножени€ с #pragma omp for
std::pair<double, std::chrono::milliseconds> mult_matrix_parallel_manual(int N, int M);
#endif // PAR_SEC_H