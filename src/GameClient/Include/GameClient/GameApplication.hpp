#pragma once

#include <Engine/Application.hpp>
#include <Engine/Buffer.hpp>
#include <Engine/Device.hpp>
#include <Engine/VertexPositionUv.hpp>
#include <Engine/GraphicsPipeline.hpp>

#include <vector>
#include <string>
#include <string_view>
#include <expected>
#include <span>
#include <memory>

class GameApplication final : public Application
{
protected:
    bool Load() override;
    void Unload() override;
    void Render() override;

private:
    std::vector<VertexPositionUv> _vertices;
    std::vector<uint32_t> _indices;

    std::unique_ptr<Buffer> _vertexBuffer;
    std::unique_ptr<Buffer> _indexBuffer;
    std::unique_ptr<GraphicsPipeline> _graphicsPipeline = {};
};