#pragma once

#include <concepts>
#include <contracts>

template <typename T>
class Tensor
{
private:
    std::vectpr<size_t> shape_;
    std::vector<size_t> strides_;
    std::unique_ptr<T[]> data_;

public:
    Tensor() = default;

    template <typename... Dims>
        requires((std::integral<Dims>) && ...)
    Tensor(Dims... dims)
    {
        if (((dims < 0) || ...))
            throw std::invalid_argument("Negative tensor dimension");
        shape_ = {static_cast<size_t>(dims)...};
        
    }

    Tensor(std::initializer_list<T> &&list)
    {
        size_ = list.size();
        data_ = std::make_unique<T[]>(list);
    }

    T const get();
    void set();

    ~Tensor() = default;
};