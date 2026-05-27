#include <iostream>
#include <microinfer/tensor.hpp>



int main() {
    std::cout << "Hello, World!" << std::endl;
    float* data = new float[4];
    data[0] = 1.0;
    data[1] = 2.0;
    data[2] = 3.0;
    data[3] = 4.0;
    Tensor<float> A(4);

    std::cout << A;

    return 0;
}

