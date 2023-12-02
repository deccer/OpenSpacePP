#include <Engine/GraphicsPipelineBuilder.hpp>
#include <Engine/GraphicsPipeline.hpp>
#include <Engine/InputLayoutElement.hpp>
#include <Engine/Io.hpp>
#include <Engine/Format.hpp>
#include <Engine/ComponentTypeClass.hpp>
#include <Engine/PrimitiveTopology.hpp>

#include <glad/glad.h>

#include <format>
#include <vector>

GraphicsPipelineBuilder::GraphicsPipelineBuilder(std::string_view label)
{
    _graphicsPipelineDescriptor._label = label;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::WithInputLayout(
    std::string_view inputLayoutLabel,
    std::span<const InputLayoutElement> inputLayoutElements)
{
    _graphicsPipelineDescriptor._inputLayoutLabel = inputLayoutLabel;
    _graphicsPipelineDescriptor._inputLayoutElements = inputLayoutElements;
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::WithPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
    _graphicsPipelineDescriptor._primitiveTopology = primitiveTopology;
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::WithShaders(
    std::string_view vertexShaderFilePath,
    std::string_view fragmentShaderFilePath)
{
    _graphicsPipelineDescriptor._vertexShaderFilePath = vertexShaderFilePath;
    _graphicsPipelineDescriptor._fragmentShaderFilePath = fragmentShaderFilePath;
    return *this;
}

std::expected<std::unique_ptr<GraphicsPipeline>, std::string> GraphicsPipelineBuilder::Build()
{
    auto graphicsPipeline = std::make_unique<GraphicsPipeline>();

    std::string vertexShaderSource;
    if (auto vertexShaderFileResult = Io::ReadTextFromFile(_graphicsPipelineDescriptor._vertexShaderFilePath))
    {
        vertexShaderSource = vertexShaderFileResult.value();
    }
    else
    {
        return std::unexpected(std::format("Unable to build graphics pipeline {}. Details: {} ",
            _graphicsPipelineDescriptor._label,
            vertexShaderFileResult.error()));
    }

    std::string fragmentShaderSource;
    if (auto fragmentShaderFileResult = Io::ReadTextFromFile(_graphicsPipelineDescriptor._fragmentShaderFilePath))
    {
        fragmentShaderSource = fragmentShaderFileResult.value();
    }
    else
    {
        return std::unexpected(std::format("Unable to build graphics pipeline {}. Details: {} ",
            _graphicsPipelineDescriptor._label,
            fragmentShaderFileResult.error()));
    }

    graphicsPipeline->_inputLayout = CreateInputLayout(
        _graphicsPipelineDescriptor._inputLayoutLabel,
        _graphicsPipelineDescriptor._inputLayoutElements);

    if (auto programResult = CreateProgram(
        std::format("Program-{}", _graphicsPipelineDescriptor._label),
        vertexShaderSource,
        fragmentShaderSource))
    {
        auto [program, vertexShader, fragmentShader] = programResult.value();
        graphicsPipeline->Program = program;
        graphicsPipeline->_vertexShader = vertexShader;
        graphicsPipeline->_fragmentShader = fragmentShader;
    }
    else
    {
        return std::unexpected(programResult.error());
    }

    return graphicsPipeline;
}

std::expected<std::tuple<uint32_t, uint32_t, uint32_t>, std::string> GraphicsPipelineBuilder::CreateProgram(
        std::string_view label,
        std::string_view vertexShaderSource,
        std::string_view fragmentShaderSource)
{
    auto vertexShaderProgram = CreateShaderProgram(std::format("{}-VS", label), GL_VERTEX_SHADER, vertexShaderSource);
    if (!vertexShaderProgram.has_value())
    {
        return std::unexpected(vertexShaderProgram.error());
    }

    auto vertexShader = vertexShaderProgram.value();

    auto fragmentShaderProgram = CreateShaderProgram(std::format("{}-FS", label), GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!fragmentShaderProgram.has_value())
    {
        return std::unexpected(fragmentShaderProgram.error());
    }

    auto fragmentShader = fragmentShaderProgram.value();

    auto id = 0u;
    glCreateProgramPipelines(1, &id);
    glObjectLabel(GL_PROGRAM_PIPELINE, id, label.size(), label.data());
    glUseProgramStages(id, GL_VERTEX_SHADER_BIT, vertexShader);
    glUseProgramStages(id, GL_FRAGMENT_SHADER_BIT, fragmentShader);

    return std::make_tuple(id, vertexShader, fragmentShader);
}

std::expected<uint32_t, std::string> GraphicsPipelineBuilder::CreateShaderProgram(
        std::string_view label,
        uint32_t shaderType,
        std::string_view shaderSource)
{
    auto shaderContent = shaderSource.data();
    auto program = glCreateShaderProgramv(shaderType, 1, &shaderContent);
    auto linkStatus = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    if (linkStatus == GL_FALSE)
    {
        auto infoLogLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        auto infoLog = std::string(infoLogLength + 1, '\0');
        glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog.data());

        return std::unexpected(std::format("Unable to link \"{}\". Details: {}", label, infoLog));
    }

    glObjectLabel(GL_PROGRAM, program, label.size(), label.data());

    return program;
}

uint32_t GraphicsPipelineBuilder::CreateInputLayout(
    std::string_view label,
    std::span<const InputLayoutElement> elements)
{
    uint32_t inputLayout;

    glCreateVertexArrays(1, &inputLayout);
    label = std::format("IL-{}", label);
    glObjectLabel(GL_VERTEX_ARRAY, inputLayout, label.size(), label.data());

    for(auto& element : elements)
    {
        auto componentCount = FormatToComponentCount(element.AttributeFormat);
        auto componentTypeClass = FormatToComponentTypeClass(element.AttributeFormat);
        auto componentIsNormalized = IsNormalizedFormat(element.AttributeFormat);

        if (componentTypeClass == ComponentTypeClass::Integer)
        {
            glVertexArrayAttribIFormat(inputLayout, element.Location, componentCount, ToGL(element.AttributeFormat), element.Offset);
        }
        else if (componentTypeClass == ComponentTypeClass::Long)
        {
            glVertexArrayAttribLFormat(inputLayout, element.Location, componentCount, ToGL(element.AttributeFormat), element.Offset);
        }
        else
        {
            glVertexArrayAttribFormat(inputLayout, element.Location, componentCount, ToGL(element.AttributeFormat), componentIsNormalized ? GL_TRUE : GL_FALSE, element.Offset);
        }
        glVertexArrayAttribBinding(inputLayout, element.Location, element.BindingIndex);
        glEnableVertexArrayAttrib(inputLayout, element.Location);        
    }

    return inputLayout;
}

uint32_t GraphicsPipelineBuilder::FormatToComponentCount(Format format)
{
    switch (format)
    {
        case Format::R8_UNORM:
        case Format::R8_SNORM:
        case Format::R16_UNORM:
        case Format::R16_SNORM:
        case Format::R16_FLOAT:
        case Format::R32_FLOAT:
        case Format::R8_SINT:
        case Format::R16_SINT:
        case Format::R32_SINT:
        case Format::R8_UINT:
        case Format::R16_UINT:
        case Format::R32_UINT:
            return 1;
        case Format::R8G8_UNORM:
        case Format::R8G8_SNORM:
        case Format::R16G16_FLOAT:
        case Format::R16G16_UNORM:
        case Format::R16G16_SNORM:
        case Format::R32G32_FLOAT:
        case Format::R8G8_SINT:
        case Format::R16G16_SINT:
        case Format::R32G32_SINT:
        case Format::R8G8_UINT:
        case Format::R16G16_UINT:
        case Format::R32G32_UINT:
            return 2;
        case Format::R8G8B8_UNORM:
        case Format::R8G8B8_SNORM:
        case Format::R16G16B16_SNORM:
        case Format::R16G16B16_FLOAT:
        case Format::R32G32B32_FLOAT:
        case Format::R8G8B8_SINT:
        case Format::R16G16B16_SINT:
        case Format::R32G32B32_SINT:
        case Format::R8G8B8_UINT:
        case Format::R16G16B16_UINT:
        case Format::R32G32B32_UINT:
            return 3;
        case Format::R8G8B8A8_UNORM:
        case Format::R8G8B8A8_SNORM:
        case Format::R16G16B16A16_UNORM:
        case Format::R16G16B16A16_FLOAT:
        case Format::R32G32B32A32_FLOAT:
        case Format::R8G8B8A8_SINT:
        case Format::R16G16B16A16_SINT:
        case Format::R32G32B32A32_SINT:
        case Format::R10G10B10A2_UINT:
        case Format::R8G8B8A8_UINT:
        case Format::R16G16B16A16_UINT:
        case Format::R32G32B32A32_UINT:
            return 4;
        default:
            return 0;
    }
}

ComponentTypeClass GraphicsPipelineBuilder::FormatToComponentTypeClass(Format format)
{
    switch (format)
    {
        case Format::R8_UNORM:
        case Format::R8_SNORM:
        case Format::R16_UNORM:
        case Format::R16_SNORM:
        case Format::R8G8_UNORM:
        case Format::R8G8_SNORM:
        case Format::R16G16_UNORM:
        case Format::R16G16_SNORM:
        case Format::R8G8B8_UNORM:
        case Format::R8G8B8_SNORM:
        case Format::R16G16B16_SNORM:
        case Format::R8G8B8A8_UNORM:
        case Format::R8G8B8A8_SNORM:
        case Format::R16G16B16A16_UNORM:
        case Format::R16_FLOAT:
        case Format::R16G16_FLOAT:
        case Format::R16G16B16_FLOAT:
        case Format::R16G16B16A16_FLOAT:
        case Format::R32_FLOAT:
        case Format::R32G32_FLOAT:
        case Format::R32G32B32_FLOAT:
        case Format::R32G32B32A32_FLOAT:
            return ComponentTypeClass::Float;
        case Format::R8_SINT:
        case Format::R16_SINT:
        case Format::R32_SINT:
        case Format::R8G8_SINT:
        case Format::R16G16_SINT:
        case Format::R32G32_SINT:
        case Format::R8G8B8_SINT:
        case Format::R16G16B16_SINT:
        case Format::R32G32B32_SINT:
        case Format::R8G8B8A8_SINT:
        case Format::R16G16B16A16_SINT:
        case Format::R32G32B32A32_SINT:
        case Format::R10G10B10A2_UINT:
        case Format::R8_UINT:
        case Format::R16_UINT:
        case Format::R32_UINT:
        case Format::R8G8_UINT:
        case Format::R16G16_UINT:
        case Format::R32G32_UINT:
        case Format::R8G8B8_UINT:
        case Format::R16G16B16_UINT:
        case Format::R32G32B32_UINT:
        case Format::R8G8B8A8_UINT:
        case Format::R16G16B16A16_UINT:
        case Format::R32G32B32A32_UINT:
            return ComponentTypeClass::Integer;
        default:
            return ComponentTypeClass::Long;
    }
}

bool GraphicsPipelineBuilder::IsNormalizedFormat(Format format)
{
    switch (format)
    {
        case Format::R8_UNORM:
        case Format::R8_SNORM:
        case Format::R16_UNORM:
        case Format::R16_SNORM:
        case Format::R8G8_UNORM:
        case Format::R8G8_SNORM:
        case Format::R16G16_UNORM:
        case Format::R16G16_SNORM:
        case Format::R8G8B8_UNORM:
        case Format::R8G8B8_SNORM:
        case Format::R16G16B16_SNORM:
        case Format::R8G8B8A8_UNORM:
        case Format::R8G8B8A8_SNORM:
        case Format::R16G16B16A16_UNORM:
            return GL_TRUE;
        case Format::R16_FLOAT:
        case Format::R32_FLOAT:
        case Format::R8_SINT:
        case Format::R16_SINT:
        case Format::R32_SINT:
        case Format::R8_UINT:
        case Format::R16_UINT:
        case Format::R32_UINT:
        case Format::R16G16_FLOAT:
        case Format::R32G32_FLOAT:
        case Format::R8G8_SINT:
        case Format::R16G16_SINT:
        case Format::R32G32_SINT:
        case Format::R8G8_UINT:
        case Format::R16G16_UINT:
        case Format::R32G32_UINT:
        case Format::R16G16B16_FLOAT:
        case Format::R32G32B32_FLOAT:
        case Format::R8G8B8_SINT:
        case Format::R16G16B16_SINT:
        case Format::R32G32B32_SINT:
        case Format::R8G8B8_UINT:
        case Format::R16G16B16_UINT:
        case Format::R32G32B32_UINT:
        case Format::R16G16B16A16_FLOAT:
        case Format::R32G32B32A32_FLOAT:
        case Format::R8G8B8A8_SINT:
        case Format::R16G16B16A16_SINT:
        case Format::R32G32B32A32_SINT:
        case Format::R10G10B10A2_UINT:
        case Format::R8G8B8A8_UINT:
        case Format::R16G16B16A16_UINT:
        case Format::R32G32B32A32_UINT:
            return GL_FALSE;
        default:
            return GL_FALSE;
    }
}

uint32_t GraphicsPipelineBuilder::ToGL(Format format)
{
    switch (format)
    {
        case Format::R8_UNORM:
        case Format::R8G8_UNORM:
        case Format::R8G8B8_UNORM:
        case Format::R8G8B8A8_UNORM:
        case Format::R8_UINT:
        case Format::R8G8_UINT:
        case Format::R8G8B8_UINT:
        case Format::R8G8B8A8_UINT:
        case Format::R8G8B8_SRGB:        
        case Format::R8G8B8A8_SRGB:
            return GL_UNSIGNED_BYTE;
        case Format::R8_SNORM:
        case Format::R8G8_SNORM:
        case Format::R8G8B8_SNORM:
        case Format::R8G8B8A8_SNORM:
        case Format::R8_SINT:
        case Format::R8G8_SINT:
        case Format::R8G8B8_SINT:
        case Format::R8G8B8A8_SINT:
            return GL_BYTE;
        case Format::R16_UNORM:
        case Format::R16G16_UNORM:
        case Format::R16G16B16A16_UNORM:
        case Format::R16_UINT:
        case Format::R16G16_UINT:
        case Format::R16G16B16_UINT:
        case Format::R16G16B16A16_UINT:
            return GL_UNSIGNED_SHORT;
        case Format::R16_SNORM:
        case Format::R16G16_SNORM:
        case Format::R16G16B16_SNORM:
        case Format::R16G16B16A16_SNORM:
        case Format::R16_SINT:
        case Format::R16G16_SINT:
        case Format::R16G16B16_SINT:
        case Format::R16G16B16A16_SINT:
            return GL_SHORT;
        case Format::R16_FLOAT:
        case Format::R16G16_FLOAT:
        case Format::R16G16B16_FLOAT:
        case Format::R16G16B16A16_FLOAT:
            return GL_HALF_FLOAT;
        case Format::R32_FLOAT:
        case Format::R32G32_FLOAT:
        case Format::R32G32B32_FLOAT:
        case Format::R32G32B32A32_FLOAT:
            return GL_FLOAT;
        case Format::R32_SINT:
        case Format::R32G32_SINT:
        case Format::R32G32B32_SINT:
        case Format::R32G32B32A32_SINT:
            return GL_INT;
        case Format::R32_UINT:
        case Format::R32G32_UINT:
        case Format::R32G32B32_UINT:
        case Format::R32G32B32A32_UINT:
            return GL_UNSIGNED_INT;
        default:
            return 0;
    }
}