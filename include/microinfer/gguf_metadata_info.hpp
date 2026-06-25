#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <variant>
#include <memory>
#include <unordered_map>

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

    struct meta_data_value;

    using meta_data_array = std::vector<meta_data_value>;

    struct meta_data_value
    {
        std::variant<
            meta_data_type,
            std::unique_ptr<meta_data_array>>
            value;
    };

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

    meta_data_value parse_value(std::ifstream &, gguf_metadata_value_type);

    meta_data_array read_metadata_array(std::ifstream &file)
    {
        uint32_t type_indentifier;
        file.read(reinterpret_cast<char *>(&type_indentifier), sizeof(type_indentifier));
        gguf_metadata_value_type array_data_type{type_indentifier};

        uint64_t array_size;
        file.read(reinterpret_cast<char *>(&array_size), sizeof(array_size));

        meta_data_array data;

        // std::cout << "array size: " << array_size << "\n";

        for (size_t i = 0; i < array_size; i++)
        {
            data.push_back(parse_value(file, array_data_type));
        }

        return data;
    }

    meta_data_value parse_value(std::ifstream &file, gguf_metadata_value_type type)
    {
        meta_data_value key;

        switch (type)
        {
        case GGUF_METADATA_VALUE_TYPE_UINT8:
            key.value = read_type<uint8_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT8:
            key.value = read_type<int8_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_UINT16:
            key.value = read_type<uint16_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT16:
            key.value = read_type<int16_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_UINT32:
            key.value = read_type<uint32_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT32:
            key.value = read_type<int32_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_UINT64:
            key.value = read_type<uint64_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_INT64:
            key.value = read_type<int64_t>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_FLOAT32:
            key.value = read_type<float>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_FLOAT64:
            key.value = read_type<double>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_BOOL:
            key.value = read_type<bool>(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_STRING:
            key.value = read_string(file);
            break;

        case GGUF_METADATA_VALUE_TYPE_ARRAY:
        {
            auto array = read_metadata_array(file);
            key.value = std::make_unique<meta_data_array>(std::move(array));
            break;
        }

        default:
            throw std::runtime_error("Unknown metadata value type");
        }

        return key;
    }

    void print_meta_data(std::string& key, meta_data_value& meta_data)
    {

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
                else if constexpr (std::is_same_v<checkDataType, std::unique_ptr<meta_data_array>>)
                {
                    std::cout << "[";
                    for (auto const &element : *data)
                    {
                        std::visit(
                            [](const auto &value)
                            {
                                using element_type = std::decay_t<decltype(value)>;

                                if constexpr (std::is_same_v<element_type, meta_data_type>)
                                {
                                    std::visit(
                                        [](auto const &primitive)
                                        {
                                            std::cout << primitive << " ";
                                        },
                                        value);
                                }
                                else
                                {
                                    std::cout << "nested array";
                                }
                            },
                            element.value);
                    }

                    std::cout << "]";
                }
            },
            meta_data.value);

        std::cout << '\n';
    }

    std::unordered_map<std::string, meta_data_value> read_metadata(std::ifstream &file, int metadata_kv_count)
    {

        
        std::unordered_map<std::string, meta_data_value> meta_data_map;
        if (!file)
            throw std::runtime_error("Failed to open file");

        for (size_t i = 0; i < metadata_kv_count; i++)
        {

            std::string key = read_string(file);

            uint32_t type_indentifier;

            file.read(reinterpret_cast<char *>(&type_indentifier), sizeof(type_indentifier));

            gguf_metadata_value_type type{type_indentifier};

            meta_data_value meta_data = parse_value(file, type);
            // print_meta_data(key, meta_data);

            meta_data_map.emplace(std::move(key), std::move(meta_data));

            

            
        }
        return meta_data_map;
    }

}