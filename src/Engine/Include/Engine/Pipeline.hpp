#pragma once

#include <cstdint>
#include <utility>

struct Buffer;

class Pipeline
{
public:
    Pipeline();
    virtual ~Pipeline();

    Pipeline(Pipeline&& other) noexcept
        : Pipeline()
    {
        swap(*this, other);
    }

    Pipeline& operator=(Pipeline other)
    {
        swap(*this, other);
        return *this;
    }

    friend void swap(Pipeline& lhs, Pipeline& rhs) noexcept
    {
        std::swap(lhs.Program, rhs.Program);
    }

    virtual void Use();

    void BindAsUniformBuffer(const Buffer& buffer, uint32_t bindingIndex, uint32_t offset, uint32_t size);
    void BindAsShaderStorageBuffer(const Buffer& buffer, uint32_t bindingIndex, uint32_t offset, uint32_t size);
    
protected:
    uint32_t Program = {};
};
