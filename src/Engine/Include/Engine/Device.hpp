#pragma once

#include <cstdint>
#include <expected>
#include <span>
#include <string>
#include <string_view>

class GraphicsPipelineBuilder;

class Device
{
public:
    Device();

    GraphicsPipelineBuilder CreateGraphicsPipelineBuilder(std::string_view label);

private:
    friend class GraphicsPipelineBuilder;
    
    uint32_t _defaultInputLayout = {};
};