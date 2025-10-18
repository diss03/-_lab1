#include <iostream>
#include <vector>
#include <string>
#include "omp_for.h"
#include "sequential.h"

//using namespace std;

int main() {
    std::string A;
    std::cout << "Enter multiplication algorithm (S, PF, PA): ";
    std::cin >> A;

    int N, M;
    std::cout << "Enter N (rows): ";
    if (!(std::cin >> N) || N <= 0) {
        std::cout << "Invalid N!" << std::endl;
        return 1;
    }
    std::cout << "Enter M (columns): ";
    if (!(std::cin >> M) || M <= 0) {
        std::cout << "Invalid M!" << std::endl;
        return 1;
    }

    if (A == "S") {
        sequential_multiplication(N, M);
    }
    else if (A == "PF") {
        parallel_for_multiplication(N, M);
    }
    else if (A == "PA") {
        std::cout << "empty function" << std::endl;
    }
    else {
        std::cout << "Instruction doesn't exist!" << std::endl;
    }

    return 0;
}