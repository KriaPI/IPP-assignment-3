#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <numeric>
#include <thread>
#include <utility>
#include <chrono>
#include <format>
#include "omp.h"
#include "exercise4.hpp"

/// @brief Solve the upper-triangular matrix equation using backward substitution.
/// @param A The coefficient matrix.
/// @param b The right hand side of the equation.
/// @param x The solution
void rowOrientedBackwardsSubstitution(const SquareMatrix<int>& A, const std::vector<int>& b, std::vector<int>& x) {
    int n = x.size();

    for (int row = n - 1; row >= 0; row--) {
        x[row] = b[row];
        for (int col = row + 1; col < n; col++) {
            x[row] -= A(row, col) * x[col];
        }
        x[row] /= A(row, row);
    }
}

void rowOrientedBackwardsSubstitutionParallel(const SquareMatrix<int>& A, const std::vector<int>& b, std::vector<int>& x) {
    int n = x.size();

    int toRemove = 0;

    #pragma omp parallel
    for (int row = n - 1; row >= 0; row--) {
        #pragma omp single 
        {
        x[row] = b[row];
        toRemove = 0;
        } 

        #pragma omp for reduction(+:toRemove) schedule(runtime)
        for (int col = row + 1; col < n; col++) {
            toRemove += A(row, col) * x[col];
        }
        
        #pragma omp single 
        {
        x[row] -= toRemove;
        x[row] /= A(row, row);
        }
    }
    
}

void benchmark(int variableCount, int maxThreads) {
    SquareMatrix<int> A (variableCount);
    A.fillUpperTriangle(1);

    std::vector<int> b(variableCount);
    std::iota(b.rbegin(), b.rend(), 1);
    std::vector<int> x(variableCount);

    std::vector<int> threads (maxThreads);
    std::iota(threads.begin(), threads.end(), 1);

    for (auto threadCount: threads) {
        std::cout << std::format("Thread count: {}\n", threadCount);
        omp_set_num_threads(threadCount);
        timeIt( [&] () { rowOrientedBackwardsSubstitutionParallel(A, b, x);});
    }
}


int main() {
    // Run with, for example OMP_SCHEDULE="dynamic" ./build/source/exercise4
    int variableCount {20};
    int maxThreadCount {16};
    benchmark(variableCount, maxThreadCount);

    return 0;
}