#include <Engine/Pipeline.hpp>
#include <Engine/Buffer.hpp>

#include <glad/glad.h>

Pipeline::Pipeline()
{
}

Pipeline::~Pipeline()
{
    if (Program != 0u)
    {
        glDeleteProgramPipelines(1, &Program);
        Program = 0u;
    }
}

void Pipeline::Use()
{
    glBindProgramPipeline(Program);
}

void Pipeline::BindAsUniformBuffer(const std::unique_ptr<Buffer>& buffer, uint32_t bindingIndex, uint32_t offset, uint32_t size)
{
    glBindBufferRange(GL_UNIFORM_BUFFER, bindingIndex, buffer->_id, offset, size);
}

void Pipeline::BindAsShaderStorageBuffer(const std::unique_ptr<Buffer>& buffer, uint32_t bindingIndex, uint32_t offset, uint32_t size)
{
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, bindingIndex, buffer->_id, offset, size);
}