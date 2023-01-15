#pragma once

#include <ShaderBase.h>

class DebugCircle : public ShaderBase
{
    friend class ThinDrawer;
    friend class CircuitUnit;
    friend class Connector;

private:
    DebugCircle(ThinDrawer* thinDrawer) : ShaderBase(thinDrawer) { }

    void prepareUniforms();
    void setupDescriptorSetLayout();
    void preparePipeline();
};