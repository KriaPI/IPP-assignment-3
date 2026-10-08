#include <numeric>
#include "omp.h"
#include "exercise4.hpp"


void columnOrientedBackwardsSubstitutionParallel(const SquareMatrix<int>& A, const std::vector<int>& b, std::vector<int>& x) {
    int n = x.size();

    for (int row = 0; row < n; row++)
    {
        x[row] = b[row];
    }
    // TODO: move the parallel directive to the outer loop and use the single clause for the first statement in the outer loop.
    #pragma omp parallel
    for (int col = n-1; col >= 0; col--) {
        #pragma omp single
        x[col] /= A(col, col);

        #pragma omp for schedule(runtime)    
        for (int row = 0; row < col; row++)
            x[row] -= A(row, col) * x[col];
    }
}

void benchmark(int variableCount, int threadCount) {
    SquareMatrix<int> A (variableCount);
    A.fillUpperTriangle(1);

    std::vector<int> b(variableCount);
    std::iota(b.rbegin(), b.rend(), 1);
    std::vector<int> x(variableCount);

    omp_set_num_threads(threadCount);
    timeIt( [&] () { columnOrientedBackwardsSubstitutionParallel(A, b, x);});
}


int main() {
    // Run with, for example OMP_SCHEDULE="dynamic" ./build/source/exercise4
    int variableCount {42000};
    int threadCount {4};
    benchmark(variableCount, threadCount);

    return 0;
}