#include <GameClient/GameApplication.hpp>
#include <Engine/Format.hpp>
#include <Engine/PrimitiveTopology.hpp>
#include <Engine/GraphicsPipelineBuilder.hpp>
#include <Engine/Utilities.hpp>

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <array>
#include <span>
#include <format>

bool GameApplication::Load()
{
    if (!Application::Load())
    {
        return false;
    }

    if (auto graphicsPipelineResult = _device->CreateGraphicsPipelineBuilder("Simple")
        .WithInputLayout("PositionUv", std::to_array<const InputLayoutElement>(
        {
            {
                .Location = 0,
                .BindingIndex = 0,
                .AttributeFormat = Format::R32G32B32_FLOAT,
                .Offset = offsetof(VertexPositionUv, Position)},
            {
                .Location = 1,
                .BindingIndex = 0,
                .AttributeFormat = Format::R32G32_FLOAT,
                .Offset = offsetof(VertexPositionUv, Uv)
            },
        }))
        .WithShaders("Data/Shaders/Simple.vs.glsl", "Data/Shaders/Simple.fs.glsl")
        .WithPrimitiveTopology(PrimitiveTopology::Triangles)
        .Build())
    {
        _graphicsPipeline = std::move(graphicsPipelineResult.value());
    }
    else
    {
        spdlog::error("Building graphics pipeline \"{}\" failed. {}", "Simple", graphicsPipelineResult.error());
        return false;
    }

    _vertices.push_back({.Position = glm::vec3(-0.5f, +0.5f, 0.0f), .Uv = glm::vec2(0.0f, 1.0f)});
    _vertices.push_back({.Position = glm::vec3(+0.0f, -0.5f, 0.0f), .Uv = glm::vec2(0.5f, 0.0f)});
    _vertices.push_back({.Position = glm::vec3(+0.5f, +0.5f, 0.0f), .Uv = glm::vec2(1.0f, 1.0f)});

    _vertexBuffer = std::make_unique<Buffer>(Buffer::Create(
        "Buffer_Vertices_Triangle",
        SizeInBytes(_vertices),        
        GL_ARRAY_BUFFER,
        GL_DYNAMIC_STORAGE_BIT));
    _vertexBuffer->Write(_vertices.data(), SizeInBytes(_vertices), 0u);

    _indices.push_back(0u);
    _indices.push_back(1u);
    _indices.push_back(2u);

    _indexBuffer = std::make_unique<Buffer>(Buffer::Create(
        "Buffer_Indices_Triangle",
        SizeInBytes(_indices),
        GL_ELEMENT_ARRAY_BARRIER_BIT,        
        GL_DYNAMIC_STORAGE_BIT));
    _indexBuffer->Write(_indices.data(), SizeInBytes(_indices), 0u);

    _graphicsPipeline->UseVertexBufferBinding(_vertexBuffer.get(), 0, 0, sizeof(VertexPositionUv));
    _graphicsPipeline->UseIndexBufferBinding(_indexBuffer.get());

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    return true;
}

void GameApplication::Unload()
{
    _graphicsPipeline.reset();
    _vertexBuffer.reset();
    _indexBuffer.reset();
    Application::Unload();
}

void GameApplication::Render()
{
    Application::Render();

    _graphicsPipeline->Use();
    _graphicsPipeline->Draw(_indices.size(), 0u);
}
