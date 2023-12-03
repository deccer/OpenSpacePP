#include <Engine/Device.hpp>
#include <Engine/GraphicsPipelineBuilder.hpp>

#include <glad/glad.h>

Device::Device()
{
    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glDisable(GL_COLOR_LOGIC_OP);

    glClearColor(0.35f, 0.67f, 0.16f, 1.0f);
    glClearDepthf(1.0f);
}

GraphicsPipelineBuilder Device::CreateGraphicsPipelineBuilder(std::string_view label)
{
    return GraphicsPipelineBuilder(label);
}