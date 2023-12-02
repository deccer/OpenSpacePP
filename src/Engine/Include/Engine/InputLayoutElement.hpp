#pragma once

enum class Format;

#include <cstdint>
#include <string_view>

struct InputLayoutElement
{
    uint32_t Location;
    uint32_t BindingIndex;    
    Format AttributeFormat;
    uint32_t Offset;
};
