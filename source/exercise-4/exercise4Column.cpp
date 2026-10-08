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

void columnOrientedBackwardsSubstitution(const SquareMatrix<int>& A, const std::vector<int>& b, std::vector<int>& x) {
    int n = x.size();

    for (int row = 0; row < n; row++)
    {
        x[row] = b[row];
    }
    for (int col = n-1; col >= 0; col--) {
        x[col] /= A(col, col);
        for (int row = 0; row < col; row++)
            x[row] -= A(row, col) * x[col];
    }
}

void columnOrientedBackwardsSubstitutionParallel(const SquareMatrix<int>& A, const std::vector<int>& b, std::vector<int>& x) {
    int n = x.size();

    for (int row = 0; row < n; row++)
    {
        x[row] = b[row];
    }
    // TODO: move the parallel directive to the outer loop and use the single clause for the first statement in the outer loop.
    #pragma omp parallel num_threads(4)
    for (int col = n-1; col >= 0; col--) {
        #pragma omp single
        x[col] /= A(col, col);

        #pragma omp for schedule(runtime)    
        for (int row = 0; row < col; row++)
            x[row] -= A(row, col) * x[col];
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
        timeIt( [&] () { columnOrientedBackwardsSubstitutionParallel(A, b, x);});
    }
}


int main() {
    // OMP_SCHEDULE="dynamic" ./build/source/exercise4

    int variableCount {20};
    int maxThreadCount {16};
    benchmark(variableCount, maxThreadCount);

    return 0;
}