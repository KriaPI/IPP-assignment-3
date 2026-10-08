#include <numeric>
#include "omp.h"
#include "exercise4.hpp"


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

void benchmark(int variableCount, int threadCount) {
    SquareMatrix<int> A (variableCount);
    A.fillUpperTriangle(1);

    std::vector<int> b(variableCount);
    std::iota(b.rbegin(), b.rend(), 1);
    std::vector<int> x(variableCount);

    omp_set_num_threads(threadCount);
    timeIt( [&] () { rowOrientedBackwardsSubstitutionParallel(A, b, x);});
}


int main() {
    // Run with, for example OMP_SCHEDULE="dynamic" ./build/source/exercise4
    int variableCount {42000};
    int threadCount {4};
    benchmark(variableCount, threadCount);

    return 0;
}