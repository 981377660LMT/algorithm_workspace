#pragma once
#include <array>
#include <vector>

// 多维动态大小数组，可以用于DP等场景。
template <typename T, size_t dimensions>
class md_vector;

namespace internal {
    template <size_t dimensions>
    size_t md_size(const array<size_t, dimensions>& dsize) {
        size_t base = 1;
        for (int i = 0; i < dimensions; i++) {
            base *= dsize[i];
        }
        return base;
    }

    template <typename T, size_t dimensions, size_t idx_dimensions>
    class md_vector_index {
    public:
        md_vector_index(md_vector<T, dimensions>& vec, size_t base = 0): vector_(vec), base_(base) {
        }

        auto operator[] (size_t v) {
            return md_vector_index<T, dimensions, idx_dimensions + 1>(vector_, (base_+v)*vector_.dsize_[idx_dimensions]);
        }

    private:
        md_vector<T, dimensions>& vector_;
        size_t base_;
    };

    template <typename T, size_t dimensions>
    class md_vector_index<T, dimensions, dimensions> {
    public:
        md_vector_index(md_vector<T, dimensions>& vec, size_t base = 0) : vector_(vec), base_(base) {
        }

        T& operator[] (size_t v) {
            return vector_.data_[base_ + v];
        }

    private:
        md_vector<T, dimensions>& vector_;
        size_t base_;
    };
}


template <typename T, size_t dimensions>
class md_vector {
public:
    md_vector(array<size_t, dimensions> dsize, T default_value = T())
        : dsize_(dsize), data_(internal::md_size(dsize), default_value)
    {
    }

    auto operator [](size_t v) {
        return internal::md_vector_index<T, dimensions, 1>(*this)[v];
    }
private:
    vector<T> data_;
    array<size_t, dimensions> dsize_;

    template <typename U, int d1, int id1>
    friend class internal::md_vector_index;
};

