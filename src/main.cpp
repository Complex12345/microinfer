#include <iostream>
#include <microinfer/tensor.hpp>
#include <microinfer/gguf_header.hpp>
#include <microinfer/gguf_tensor_info.hpp>
#include <filesystem>

int main() {
    // microinfer::Tensor<float> A(4);
    // A[0] = 1.0f;
    // A[1] = 2.0f;
    // A[2] = 3.0f;
    // A[3] = 4.0f;

    // std::cout << A << "\n";

    
    // microinfer::Tensor<float> B = {5.0f, 6.0f, 7.0f, 8.0f};
    // std::cout << B << "\n";

    
    // A.set(0, 99.0f);
    // std::cout << "A[0] check: " << A.get(0) << "\n";

    // microinfer::Tensor<float> C(2, 3);
    // for (size_t i = 0; i < C.size(); ++i) C[i] = (float)i;
    // std::cout << C << "\n";

    std::filesystem::path const file_path = "../model/tinyllama-1.1b-chat-v1.0.Q4_K_M.gguf";
    std::ifstream file(file_path, std::ios::binary);

    if(!file) throw std::runtime_error("Failed to open file");

    microinfer::GGUFHeader gguf_header = microinfer::read_header(file);

    std::cout << "GGUF contents:\n";
    std::cout << "version: " << gguf_header.version << "\n";
    std::cout << "Tensor count: " << gguf_header.tensor_count << "\n";
    std::cout << "Metadata kv count: " << gguf_header.metadata_kv_count << "\n";

    microinfer::read_metadata(file, gguf_header.metadata_kv_count);









    



    file.close();
    return 0;
}