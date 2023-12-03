#version 460 core

layout (location = 0) out gl_PerVertex
{
    vec4 gl_Position;
};

layout(location = 0) out vec2 v_uv;

struct Vertex
{
    float Position[3];
    float Uv[2];
};

layout(std430, binding = 0) restrict readonly buffer VertexBuffer { Vertex Vertices[]; };
layout(std430, binding = 1) restrict readonly buffer IndexBuffer { uint Indices[]; };

void main()
{
    Vertex vertex = Vertices[Indices[gl_VertexID]];
    vec3 position = vec3(vertex.Position[0], vertex.Position[1], vertex.Position[2]);
    vec2 uv = vec2(vertex.Uv[0], vertex.Uv[1]);

    gl_Position = vec4(position, 1.0);
    v_uv = uv;
}