#include <Engine/Device.hpp>
#include <Engine/GraphicsPipelineBuilder.hpp>

GraphicsPipelineBuilder Device::CreateGraphicsPipelineBuilder(std::string_view label)
{
    auto graphicsPipelineBuilder = GraphicsPipelineBuilder(label);
    return graphicsPipelineBuilder;
}