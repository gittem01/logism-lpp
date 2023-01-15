#version 460 core

layout (location = 0) out vec4 FragColour;

layout (location = 0) in vec4 localPos;

layout (set = 0, binding = 2) uniform UBO
{
    vec4 colour;
    vec4 outerColour;
    vec4 data; // zoom, lineThickness, outerSmoothThicknessRatio, reversed resizeAmount(1 / scale)
} ubo;

float lineRel(vec2 a, vec2 b, vec2 c)
{
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void main()
{
    float adjustedOuterLineWidth;
    if (ubo.data.w > 0.0f)
    {
        adjustedOuterLineWidth = ubo.data.y;
    }
    else
    {
        adjustedOuterLineWidth = ubo.data.y / (ubo.data.x * ubo.data.x);
    }

    float len = length(localPos) * 2.0f;
    if (len < 1.0f - adjustedOuterLineWidth)
    {
        FragColour = ubo.colour;
    }
    if (len < 1.0f)
    {
        float midDist = 1 - adjustedOuterLineWidth * 0.5f;
        float distFromMidDist = len - midDist;
        if (len >= midDist)
        {
            float perct = distFromMidDist / (adjustedOuterLineWidth * 0.5f);
            float reverseOuter = 1.0f - ubo.data.z;
            if (perct > reverseOuter)
            {
                perct = (perct - reverseOuter) * (1.0f / ubo.data.z);
                FragColour = vec4(ubo.outerColour.xyz, (1 - perct) * ubo.outerColour.w);
            }
            else
            {
                FragColour = ubo.outerColour;
            }
        }
        else if (len >= 1 - adjustedOuterLineWidth)
        {
            float perct = -distFromMidDist / (adjustedOuterLineWidth * 0.5f);
            float reverseOuter = 1.0f - ubo.data.z;
            if (perct > reverseOuter)
            {
                perct = (perct - reverseOuter) * (1.0f / ubo.data.z);
                float alphaVal = ubo.outerColour.w + perct * (ubo.colour.w - ubo.outerColour.w);
                vec3 colourMix = ubo.outerColour.xyz * (1 - perct * ubo.colour.w / alphaVal) + ubo.colour.xyz * perct * ubo.colour.w / alphaVal;
                FragColour = vec4(colourMix, alphaVal);
            }
            else
            {
                FragColour = ubo.outerColour;
            }
        }
    }
    else
    {
        FragColour = vec4(0, 0, 0, 0);
    }
}