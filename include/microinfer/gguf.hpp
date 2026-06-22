#include <filesystem>
#include <fstream>
#include <cstdint>
#include <stdexcept>

namespace microinfer
{
    struct GGUFHeader
    {
        uint32_t version;
        uint64_t tensor_count;
        uint64_t metadata_kv_count;
    };

    GGUFHeader read_header(const std::filesystem::path &path)
    {
        std::ifstream file(path, std::ios::binary);

        if (!file)
            throw std::runtime_error("Failed to open file");

        char isGGUF[4];
        file.read(isGGUF, 4);
        if (std::string(isGGUF, 4) != "GGUF")
            throw std::runtime_error("Not a GGUF file");

        GGUFHeader gguf_header;

        file.read(reinterpret_cast<char *>(&gguf_header.version), sizeof(gguf_header.version));
        file.read(reinterpret_cast<char *>(&gguf_header.tensor_count), sizeof(gguf_header.tensor_count));
        file.read(reinterpret_cast<char *>(&gguf_header.metadata_kv_count), sizeof(gguf_header.metadata_kv_count));

        return gguf_header;
    }
}