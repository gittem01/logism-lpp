#pragma once

#include <ShaderBase.h>

class DebugLine : public ShaderBase
{
    friend class ThinDrawer;
    friend class CircuitUnit;
    friend class Line;

private:
    DebugLine(ThinDrawer* thinDrawer) : ShaderBase(thinDrawer) { }

    void prepareUniforms();
    void setupDescriptorSetLayout();
    void preparePipeline();
};