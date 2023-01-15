#pragma once

#include <ShaderBase.h>

class DebugPolygon : public ShaderBase
{
    friend class ThinDrawer;
    friend class CircuitUnit;

private:
    DebugPolygon(ThinDrawer* thinDrawer) : ShaderBase(thinDrawer) { }

    void prepareVertexData();
    void prepareUniforms();
    void setupDescriptorSetLayout();
    void preparePipeline();
};