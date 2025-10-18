//#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <chrono>
#include "omp_for.h"
#include "sequential.h"

using namespace std;

int main() {
    int testCount;
    cout << "Enter the number of tests: ";
    if (!(cin >> testCount) || testCount <= 0) {
        cout << "Invalid number of tests!" << endl;
        return 1;
    }

    vector<int> N_values(testCount);
    vector<int> M_values(testCount);

    for (int i = 0; i < testCount; ++i) {
        cout << "Enter N (rows) for test " << i + 1 << ": ";
        if (!(cin >> N_values[i]) || N_values[i] <= 0) {
            cout << "Invalid N for test " << i + 1 << "!" << endl;
            return 1;
        }
        cout << "Enter M (columns) for test " << i + 1 << ": ";
        if (!(cin >> M_values[i]) || M_values[i] <= 0) {
            cout << "Invalid M for test " << i + 1 << "!" << endl;
            return 1;
        }
    }

    string A;
    cout << "Enter multiplication algorithm (S, PF, PA): ";
    cin >> A;

    for (int test = 0; test < testCount; ++test) {
        int N = N_values[test];
        int M = M_values[test];
        cout << "\nTest " << test + 1 << " (N=" << N << ", M=" << M << "):" << endl;

        double totalTime = 0.0;  // В миллисекундах
        double totalSum = 0.0;

        // Цикл из 10 итераций для текущего теста
        for (int iteration = 0; iteration < 10; ++iteration) {
            pair<double, chrono::milliseconds> result;

            if (A == "S") {
                result = sequential_multiplication(N, M);
                cout << iteration + 1 << " multiplication result " << result.first << endl;
            }
            else if (A == "PF") {
                result = parallel_for_multiplication(N, M);
                cout << iteration + 1 << " multiplication result " << result.first << endl;
            }
            else if (A == "PA") {
                cout << "empty function (iteration " << iteration + 1 << ")" << endl;
                continue;
            }
            else {
                cout << "Instruction doesn't exist!" << endl;
                return 1;
            }

            // Накопление результатов
            totalTime += result.second.count();     // Время в миллисекундах
            cout << "Iteration " << iteration + 1 << ": Time = " << result.second.count()
                << " ms" << endl;
        }

        // Вывод усреднённых результатов
        cout << "Average time for test " << test + 1 << ": " << totalTime / 10 << " ms" << endl;
    }

    return 0;
}