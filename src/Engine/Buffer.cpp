#include <Engine/Buffer.hpp>

#include <glad/glad.h>

#include <cassert>

Buffer Buffer::Create(
    std::string_view label,
    uint32_t size,
    uint32_t type,
    uint32_t storage,
    bool isMapped) noexcept
{
    auto buffer = Buffer();
    glCreateBuffers(1, &buffer._id);
    glNamedBufferStorage(buffer._id, size, nullptr, storage);

    glObjectLabel(GL_BUFFER, buffer._id, label.size(), label.data());

    if (isMapped)
    {
        buffer._mappedMemory = glMapNamedBuffer(buffer._id, GL_READ_WRITE);
    }

    buffer._type = type;
    buffer._size = size;
    return buffer;
}

Buffer::~Buffer()
{
    if (_mappedMemory != nullptr)
    {
        glUnmapNamedBuffer(_id);
    }
    if (_id)
    {
        glDeleteBuffers(1, &_id);
    }
}

Buffer::Buffer(Buffer&& other) noexcept
{
    Swap(other);
}

Buffer& Buffer::operator =(Buffer&& other) noexcept
{
    Buffer(std::move(other)).Swap(*this);
    return *this;
}

void Buffer::Swap(Buffer& other) noexcept
{
    using std::swap;
    swap(_id, other._id);
    swap(_size, other._size);
    swap(_mappedMemory, other._mappedMemory);
}

void Buffer::Write(const void* data, uint64_t size, uint64_t offset) const noexcept
{
    assert(offset + size <= _size && "overflow");
    if (size == 0)
    {
        return;
    }
    glNamedBufferSubData(_id, offset, size, data);
}