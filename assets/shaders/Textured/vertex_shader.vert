#version 460 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 i_texCoord;

layout (location = 0) out vec2 o_texCoord;

layout (set = 0, binding = 0) uniform SharedUboData
{
	mat4 orthoMatrix;
} sharedUboData;

layout (set = 0, binding = 1) uniform UBO
{
	mat4 modelMatrix;
} ubo;

void main() 
{
	o_texCoord = i_texCoord;
	vec4 wp = ubo.modelMatrix * vec4(inPos.xy, 0.0, 1.0f);

	gl_Position = sharedUboData.orthoMatrix * wp;
}