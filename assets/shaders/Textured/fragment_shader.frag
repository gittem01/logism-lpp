#version 460 core

layout (location = 0) out vec4 outFragColor;

layout (location = 0) in vec2 texCoord;

layout(set = 1, binding = 0) uniform sampler2D tex;

float lineRel(vec2 a, vec2 b, vec2 c)
{
     return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void main()
{
	outFragColor = texture(tex, texCoord);
}