#include <cstddef>
#include <iostream>
#include <utility>

template <typename T>
class SquareMatrix {
    std::size_t m_dimension{0};
    T* m_data{};

   public:
    explicit SquareMatrix(std::size_t dimension) : m_dimension(dimension) {
        m_data = new T[dimension * dimension];
    }

    /// Note: this provides no checking for size and assumes that the rows are
    /// equal in length to number of rows (rows == columns).
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

    [[nodiscard]] std::size_t dimension() const noexcept { return m_dimension; }

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

   private:
    void moveFrom(SquareMatrix& source) {
        m_dimension = std::exchange(source.m_dimension, 0);
        m_data = std::exchange(source.m_data, nullptr);
    }
};

template <typename T>
SquareMatrix<T> multiplyMatrices(const SquareMatrix<T>& A,
                                 const SquareMatrix<T>& B) {
    SquareMatrix<T> C(A.dimension());
    auto dim = A.dimension();
    std::size_t i = 0;
    std::size_t j = 0;
    std::size_t k = 0;

    // #pragma omp parallel default(private) shared(A, B, C, dim) num_threads(4)
    // #pragma omp for schedule(static)
    // #pragma opm for
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            C(i, j) = 0;
            for (k = 0; k < dim; k++) {
                C(i, j) += A(i, k) * B(k, j);
            }
        }
    }
    return C;
}

int main() {
    SquareMatrix A{
        {1, 2, 3},
        {2, 3, 4},
        {3, 4, 5}
    };

    SquareMatrix B{
        {2, 0,0},
        {0, 2,0 },
        {0, 0, 2}
    };

    std::cout << multiplyMatrices(A, B);

    return 0;
}