#pragma once

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


template <typename T>
void timeIt(T function) {
    auto start {std::chrono::system_clock::now()};
    function();
    std::chrono::duration<double> duration {std::chrono::system_clock::now() - start};
    std::cout << std::format("Duration: {:.8f} seconds\n", duration.count());
}