#pragma once

#include <ShaderHeaders/DebugCircle.h>
#include <ShaderHeaders/DebugLine.h>
#include <ShaderHeaders/DebugPolygon.h>
#include <Sim.h>
#include <vector>


typedef enum unit_type
{
    BOX     = 0x0001,
    DOT     = BOX << 1,
    LINE    = DOT << 1,
} unit_type;

class ThinDrawer;
class Sim;
class Connector;

class CircuitUnit
{
public:
    unit_type unitType = BOX;
    ShaderBase* body;
    grid_vector position;
    grid_vector size;
    std::vector<Connector*> connectors;
    Sim* sim;

    CircuitUnit(Sim* sim);
    CircuitUnit();

    virtual void prepareVertexData();
    virtual void prepareUniforms();
    virtual void setupDescriptorSetLayout();
    virtual void preparePipeline();
    virtual void setupDescriptorSet();

    virtual void fillUniforms(int frameNum);

    virtual void prepareRender(int frameNum);
};