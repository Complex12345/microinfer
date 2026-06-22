#pragma once

#include <fstream>
#include <iostream>
#include <vector>

namespace microinfer
{

    enum gguf_metadata_value_type : uint32_t
    {
        GGUF_METADATA_VALUE_TYPE_UINT8 = 0,
        GGUF_METADATA_VALUE_TYPE_INT8 = 1,
        GGUF_METADATA_VALUE_TYPE_UINT16 = 2,
        GGUF_METADATA_VALUE_TYPE_INT16 = 3,
        GGUF_METADATA_VALUE_TYPE_UINT32 = 4,
        GGUF_METADATA_VALUE_TYPE_INT32 = 5,
        GGUF_METADATA_VALUE_TYPE_FLOAT32 = 6,
        GGUF_METADATA_VALUE_TYPE_BOOL = 7,
        GGUF_METADATA_VALUE_TYPE_STRING = 8,
        GGUF_METADATA_VALUE_TYPE_ARRAY = 9,
        GGUF_METADATA_VALUE_TYPE_UINT64 = 10,
        GGUF_METADATA_VALUE_TYPE_INT64 = 11,
        GGUF_METADATA_VALUE_TYPE_FLOAT64 = 12,
    };

    using meta_data_type = std::variant<
        uint8_t,
        int8_t,
        uint16_t,
        int16_t,
        uint32_t,
        int32_t,
        uint64_t,
        int64_t,
        float,
        double,
        bool,
        std::string>;

    using meta_data_value = std::variant<
        meta_data_type,
        std::vector<meta_data_type>>;

    template <typename T>
    T read_type(std::ifstream &file)
    {
        T value;
        file.read(reinterpret_cast<char *>(&value), sizeof(value));
        return value;
    }

    std::string read_string(std::ifstream &file)
    {
        uint64_t len;
        file.read(reinterpret_cast<char *>(&len), sizeof(len));
        std::string str(len, '\0');
        file.read(str.data(), len);
        return str;
    }

    void read_metadata_array(std::ifstream &file)
    {
        uint64_t key_length;
    }

    meta_data_value parse_value(std::ifstream &file, gguf_metadata_value_type type)
    {

        meta_data_value value;
        switch (type)
        {
        case GGUF_METADATA_VALUE_TYPE_UINT8:
            value = read_type<uint8_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT8:
            value = read_type<int8_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_UINT16:
            value = read_type<uint16_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT16:
            value = read_type<int16_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_UINT32:
            value = read_type<uint32_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT32:
            value = read_type<int32_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_UINT64:
            value = read_type<uint64_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT64:
            value = read_type<int64_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_FLOAT32:
            value = read_type<float>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_FLOAT64:
            value = read_type<double>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_BOOL:
            value = read_type<bool>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_STRING:
            value = read_string(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_ARRAY:

            break;

        default:
            throw std::runtime_error("Unknown GGUF metadata value type");
        }
        return value;
    }

    void read_metadata(std::ifstream &file, int metadata_kv_count)
    {

        if (!file)
            throw std::runtime_error("Failed to open file");

        for (size_t i = 0; i < metadata_kv_count; i++)
        {

            std::string key = read_string(file);

            uint32_t type_indentifier;

            file.read(reinterpret_cast<char *>(&type_indentifier), sizeof(type_indentifier));

            gguf_metadata_value_type type{type_indentifier};

            meta_data_value value = parse_value(file, type);

            std::cout << key << ": ";

            std::visit(
                [](const auto &data)
                {
                    using checkDataType = std::decay_t<decltype(data)>;

                    
                    if constexpr (std::is_same_v<checkDataType, meta_data_type>)
                    {
                        std::visit(
                            [](const auto &value)
                            {
                                std::cout << value;
                            },
                            data);
                    }
                    else
                    {
                        std::cout << "not implemented";
                    }
                },
                value);

            std::cout << '\n';
        }
    }

}