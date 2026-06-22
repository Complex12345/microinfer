#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <ostream>
#include <string>

namespace microinfer
{

    template <typename T>
    class Tensor
    {
    private:
        std::vector<size_t> shape_;
        std::vector<size_t> strides_;
        std::unique_ptr<T[]> data_;
        size_t size_ = 0;

        void compute_strides()
        {
            strides_.resize(shape_.size());
            if (shape_.empty())
                return;
            strides_.back() = 1;
            for (int i = (int)shape_.size() - 2; i >= 0; --i)
                strides_[i] = strides_[i + 1] * shape_[i + 1];
        }

    public:
        Tensor() = default;

        template <typename... Dims>
            requires((std::integral<Dims>) && ...)
        Tensor(Dims... dims)
        {
            if (((dims < 0) || ...))
                throw std::invalid_argument("Negative tensor dimension");
            shape_ = {static_cast<size_t>(dims)...};
            size_ = 1;
            for (auto d : shape_)
                size_ *= d;
            compute_strides();
            data_ = std::make_unique<T[]>(size_);
        }

        Tensor(std::initializer_list<T> list)
        {
            size_ = list.size();
            shape_ = {size_};
            compute_strides();
            data_ = std::make_unique<T[]>(size_);
            std::copy(list.begin(), list.end(), data_.get());
        }

        T const &get(size_t idx) const
        {
            if (idx >= size_)
                throw std::out_of_range("Index " + std::to_string(idx) +
                                        " out of range for size " + std::to_string(size_));
            return data_[idx];
        }

        void set(size_t idx, T val)
        {
            if (idx >= size_)
                throw std::out_of_range("index " + std::to_string(idx) +
                                        " out of range for size " + std::to_string(size_));
            data_[idx] = val;
        }

        friend std::ostream &operator<<(std::ostream &os, const Tensor<T> &t)
        {
            os << "Tensor(shape=[";
            for (size_t i = 0; i < t.shape_.size(); ++i)
            {
                os << t.shape_[i];
                if (i + 1 < t.shape_.size())
                    os << ", ";
            }
            os << "], data=[";
            for (size_t i = 0; i < t.size_; ++i)
            {
                os << t.data_[i];
                if (i + 1 < t.size_)
                    os << ", ";
            }
            os << "])";
            return os;
        }

        T &operator[](size_t idx) { return data_[idx]; }
        const T &operator[](size_t idx) const { return data_[idx]; }

        size_t size() const { return size_; }
        const std::vector<size_t> &shape() const { return shape_; }
        const std::vector<size_t> &strides() const { return strides_; }
        T *data() { return data_.get(); }
        const T *data() const { return data_.get(); }

        ~Tensor() = default;
    };

}