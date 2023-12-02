#pragma once

#include <cstdint>
#include <expected>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <tuple>

enum class ComponentTypeClass;
enum class Format;
enum class PrimitiveTopology;
struct InputLayoutElement;

class GraphicsPipeline;
class GraphicsPipelineBuilder;

class GraphicsPipelineDescriptor
{
public:
private:
    friend class GraphicsPipelineBuilder;

    std::string_view _label;
    PrimitiveTopology _primitiveTopology;
    std::string_view _inputLayoutLabel;    
    std::span<const InputLayoutElement> _inputLayoutElements;
    std::string_view _vertexShaderFilePath;
    std::string_view _fragmentShaderFilePath;
};

class GraphicsPipelineBuilder
{
public:
    GraphicsPipelineBuilder(std::string_view label);

    GraphicsPipelineBuilder& WithInputLayout(
        std::string_view inputLayoutLabel,
        std::span<const InputLayoutElement> inputLayoutElements);
    GraphicsPipelineBuilder& WithPrimitiveTopology(PrimitiveTopology primitiveTopology);
    GraphicsPipelineBuilder& WithShaders(
        std::string_view vertexShaderFilePath,
        std::string_view fragmentShaderFilePath);

    std::expected<std::unique_ptr<GraphicsPipeline>, std::string> Build();

private:
    GraphicsPipelineDescriptor _graphicsPipelineDescriptor;

    std::expected<std::tuple<uint32_t, uint32_t, uint32_t>, std::string> CreateProgram(
        std::string_view label,
        std::string_view vertexShaderSource,
        std::string_view fragmentShaderSource);
    std::expected<uint32_t, std::string> CreateShaderProgram(
        std::string_view label,
        uint32_t shaderType,
        std::string_view shaderSource);
    uint32_t CreateInputLayout(
        std::string_view label,
        std::span<const InputLayoutElement> elements);
        
    uint32_t FormatToComponentCount(Format format);
    ComponentTypeClass FormatToComponentTypeClass(Format format);
    bool IsNormalizedFormat(Format format);

    uint32_t ToGL(Format format);
    uint32_t ToGL(PrimitiveTopology PrimitiveTopology);
};