#pragma once

#include <cstdint>
#include <string_view>
#include <utility>

class Pipeline;

class Buffer
{
public:
    static Buffer Create(
        std::string_view label,
        uint32_t size,
        uint32_t type,
        uint32_t storage,
        bool isMapped = false) noexcept;

    Buffer() noexcept = default;
    ~Buffer();

    Buffer(const Buffer&) noexcept = delete;
    Buffer& operator =(const Buffer&) noexcept = delete;
    Buffer(Buffer&& other) noexcept;
    Buffer& operator =(Buffer&& other) noexcept;

    void Swap(Buffer& other) noexcept;

    void Write(const void* data, uint64_t size, uint64_t offset) const noexcept;

private:
    friend class Pipeline;
    friend class GraphicsPipeline;

    uint32_t _id = 0;
    uint32_t _size = 0;
    uint32_t _type = 0;
    void* _mappedMemory = nullptr;
};