#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
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


   private:
    void moveFrom(SquareMatrix& source) {
        m_dimension = std::exchange(source.m_dimension, 0);
        m_data = std::exchange(source.m_data, nullptr);
    }
};




int main() {
    SquareMatrix<int> A {
        {2, -3, 0},
        {0, 1, 1},
        {0, 0, -5},
    };

    std::array<int, 3> b{};
    std::array<int, 3> x{};

    int row = 0;
    int col = 0;
    int n = 3;

    for (row = n-1; row >= 0; row--) {
        x[row] = b[row];
        for (col = row-1; col < n; col++)
            x[row] -= A(row, col) * x[col];
        x[row] /= A(row, col);
}

    std::cout << std::format("Solutions: [{}, {}, {}]", x[0], x[1], x[2]);

    return 0;
}