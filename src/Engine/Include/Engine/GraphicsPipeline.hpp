#pragma once

#include <Engine/Pipeline.hpp>
#include <Engine/InputLayoutElement.hpp>

class Buffer;

class GraphicsPipeline : public Pipeline
{
public:
    GraphicsPipeline() : Pipeline()
    {
    }

    ~GraphicsPipeline() override;

    GraphicsPipeline(GraphicsPipeline& other)
        : Pipeline()
    {
        swap(*this, other);
    }

    GraphicsPipeline(GraphicsPipeline&& other) noexcept
        : Pipeline()
    {
        swap(*this, other);
    }

    GraphicsPipeline& operator=(GraphicsPipeline other)
    {
        swap(*this, other);
        return *this;
    }

    friend void swap(GraphicsPipeline& lhs, GraphicsPipeline& rhs) noexcept
    {
        using std::swap;
        swap(lhs._inputLayout, rhs._inputLayout);
        swap(lhs._vertexShader, rhs._vertexShader);
        swap(lhs._fragmentShader, rhs._fragmentShader);
        swap(lhs._primitiveTopology, rhs._primitiveTopology);
    }

    void UseVertexBufferBinding(
        const Buffer* vertexBuffer,
        uint32_t bindingIndex,
        uint32_t offset,
        uint32_t stride);
    void UseIndexBufferBinding(const Buffer* indexBuffer);
    void Use() override;

    void DrawArrays(
        uint32_t elementCount,
        uint32_t elementOffset = 0);
    void DrawElements(
        uint32_t elementCount,
        uint32_t offsetInBytes = 0);

private:
    friend class GraphicsPipelineBuilder;

    uint32_t _inputLayout = {};
    uint32_t _vertexShader = {};
    uint32_t _fragmentShader = {};
    uint32_t _primitiveTopology = {};
};