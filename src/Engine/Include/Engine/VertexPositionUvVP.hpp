#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

/*
struct VertexPositionUvVp
{
    float Position[3];
    float Uv[2];
};
*/

struct VertexPositionUvVp
{
    glm::vec3 Position;
    glm::vec2 Uv;
};