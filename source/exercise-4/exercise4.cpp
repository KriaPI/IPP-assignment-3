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

template <typename T>
class SquareMatrix {
    std::size_t m_dimension{0};
    T* m_data{};

   public:
    SquareMatrix() = delete;

    /// @brief Initialize the matrix of size dimension x dimension with 0.
    explicit SquareMatrix(std::size_t dimension) : m_dimension(dimension), m_data(new T[dimension * dimension]) {
        
        fill(0);
    }

    /// Note: this provides no checking for size and assumes that the rows are
    /// equal in length to number of rows (rows == columns).

    /// @brief Initialize the matrix of size dimension x dimension with the given 
    // initializer list.
    SquareMatrix(std::initializer_list<std::initializer_list<T>> initList) {
        auto dimension{initList.size()};
        m_dimension = dimension;
        m_data = new T[dimension * dimension];

        std::size_t i = 0;
        for (const auto& row : initList) {
            for (const auto& element : row) {
                m_data[i] = element;
                ++i;
            }
        }
    }

    ~SquareMatrix() { delete[] m_data; }

    SquareMatrix(SquareMatrix&& source) noexcept { moveFrom(source); }
    SquareMatrix& operator=(SquareMatrix&& rhs) noexcept {
        if (this == &rhs) {
            return *this;
        }

        moveFrom(rhs);
        return *this;
    }

    SquareMatrix(SquareMatrix& other) = delete;
    SquareMatrix& operator=(const SquareMatrix&) = delete;

    [[nodiscard]] T& operator()(const std::size_t row, const std::size_t column) {
        return m_data[row * dimension() + column];
    }

    [[nodiscard]] const T& operator()(const std::size_t row, const std::size_t column) const {
        return m_data[row * dimension() + column];
    }
    friend std::ostream& operator<<(std::ostream& out, const SquareMatrix& matrix) {
        out << "[\n";
        int dimension = matrix.dimension();
        for (auto i = 0; i < dimension; ++i) {
            for (auto j = 0; j < dimension; ++j) {
                out << matrix(i, j) << " ";
            }
            out << "\n";
        }
        out << "]\n";
        return out;
    }

    [[nodiscard]] std::size_t dimension() const noexcept { return m_dimension; }
    [[nodiscard]] std::size_t size() const noexcept { return m_dimension * m_dimension; }


    /// @brief Assign all elements in the matrix to value.
    void fill(T value) {
        std::fill_n(m_data, size(), value);
    }

    /// @brief Assign all elements on the diagonal in the matrix to value.
    void fillDiagonal(T value) {
        for (std::size_t i = 0; i < dimension(); ++i) {
            this->operator()(i, i) = value;
        }
    }

    void fillUpperTriangle(T value) {
        for (std::size_t row = 0; row < dimension(); ++row) {
            std::size_t start {row};
            std::size_t end {dimension()};
            
            for (std::size_t column = start; column < end; ++column) {
                this->operator()(row, column) = value;
            }
        }
    }


   private:
    void moveFrom(SquareMatrix& source) {
        m_dimension = std::exchange(source.m_dimension, 0);
        m_data = std::exchange(source.m_data, nullptr);
    }
};

/// @brief Solve the upper-triangular matrix equation using backward substitution.
/// @param A The coefficient matrix.
/// @param b The right hand side of the equation.
/// @param x The solution
void rowOrientedBackwardsSubstitution(const SquareMatrix<int>& A, const std::vector<int>& b, std::vector<int>& x) {
    int n = x.size();

    for (int row = n - 1; row >= 0; row--) {
        x[row] = b[row];
        // TODO: parallelize this loop using omp for and a reduction clause
        for (int col = row + 1; col < n; col++) {
            x[row] -= A(row, col) * x[col];
        }
        x[row] /= A(row, row);
    }
}

void rowOrientedBackwardsSubstitutionParallel(const SquareMatrix<int>& A, const std::vector<int>& b, std::vector<int>& x) {
    int n = x.size();

    for (int row = n - 1; row >= 0; row--) {
        x[row] = b[row];
        // TODO: parallelize this loop using omp for and a reduction clause

        auto toRemove {0};

        #pragma omp parallel for default(private) shared(A, x, row, n) reduction(+:toRemove) schedule(runtime)
        for (int col = row + 1; col < n; col++) {
            toRemove += A(row, col) * x[col];
        }

        x[row] -= toRemove;
        x[row] /= A(row, row);
    }
}

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

    // TODO: figure out how to parallelize and do it.

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

template <typename T>
void timeIt(T function) {
    auto start {std::chrono::system_clock::now()};
    function();
    std::chrono::duration<double> duration {std::chrono::system_clock::now() - start};
    std::cout << std::format("Duration: {:.8f} seconds\n", duration.count());
}

void benchmark(int variableCount) {
    SquareMatrix<int> A (variableCount);
    A.fillUpperTriangle(1);

    std::vector<int> b(variableCount);
    std::iota(b.rbegin(), b.rend(), 1);
    std::vector<int> x(variableCount);

    int maxThreads {4};
    std::vector<int> threads (maxThreads);
    std::iota(threads.begin(), threads.end(), 1);

    for (auto threadCount: threads) {
        std::cout << std::format("Thread count: {}\n", threadCount);
        omp_set_num_threads(threadCount);
        timeIt( [&] () { rowOrientedBackwardsSubstitutionParallel(A, b, x);});
    }
}


int main() {
    int variableCount {20};

    SquareMatrix<int> A (variableCount);
    A.fillUpperTriangle(1);

    std::vector<int> b(variableCount);
    std::iota(b.rbegin(), b.rend(), 1);
    std::vector<int> x(variableCount);

    
    columnOrientedBackwardsSubstitution(A, b, x);
    
    std::cout << A << "\n";
    for (auto i: x) {
       std::cout << std::format("{} ", i);
    }
    std::cout << "\n";

    // OMP_SCHEDULE="dynamic" ./build/source/exercise4

    return 0;
}