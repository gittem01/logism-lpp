#version 460 core

layout (location = 0) in vec2 vertex;

layout (location = 0) out vec4 localPos;

layout (set = 0, binding = 0) uniform SharedUboData
{
    mat4 orthoMatrix;
} sharedUboData;

layout (set = 0, binding = 1) uniform UBO
{
    mat4 modelMatrix;
} ubo;

layout (set = 0, binding = 2) uniform UBO2
{
    vec4 colour;
    vec4 outerColour;
    vec4 data; // zoom, lineThickness, outerSmoothThicknessRatio, unused
} uboF;

void main()
{
    vec4 pos;
    if (uboF.data.w > 0.0f)
    {
        pos = sharedUboData.orthoMatrix * ubo.modelMatrix * vec4(vertex / (uboF.data.x * uboF.data.x), 0, 1);
    }
    else
    {
        pos = sharedUboData.orthoMatrix * ubo.modelMatrix * vec4(vertex, 0, 1);
    }

    gl_Position = pos;

    vec4 wp = ubo.modelMatrix * vec4(vertex, 0.0, 1.0f);
    localPos = vec4(vertex, 0, 0);
}