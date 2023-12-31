#include <Engine/GraphicsPipeline.hpp>
#include <Engine/Buffer.hpp>

#include <glad/glad.h>

GraphicsPipeline::~GraphicsPipeline()
{
    if (_vertexShader != 0u)
    {
        glDeleteProgram(_vertexShader);
        _vertexShader = 0u;
    }

    if (_fragmentShader != 0u)
    {
        glDeleteProgram(_fragmentShader);
        _fragmentShader = 0u;
    }
}

void GraphicsPipeline::UseVertexBufferBinding(
    const Buffer* vertexBuffer,
    uint32_t bindingIndex,
    uint32_t offset,
    uint32_t stride)
{
    glVertexArrayVertexBuffer(_inputLayout, bindingIndex, vertexBuffer->_id, offset, stride);
}

void GraphicsPipeline::UseIndexBufferBinding(const Buffer* indexBuffer)
{
    glVertexArrayElementBuffer(_inputLayout, indexBuffer->_id);
}

void GraphicsPipeline::Use()
{
    Pipeline::Use();
    glBindVertexArray(_inputLayout);
}

void GraphicsPipeline::DrawArrays(
    uint32_t elementCount,
    uint32_t elementOffset)
{
    glDrawArrays(_primitiveTopology, elementOffset, elementCount);
}

void GraphicsPipeline::DrawElements(
    uint32_t elementCount,
    uint32_t offsetInBytes)
{
    glDrawElements(_primitiveTopology, elementCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(offsetInBytes));
}
