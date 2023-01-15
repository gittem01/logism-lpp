#include <ShaderHeaders/DebugLine.h>
#include <glm/glm.hpp>
#include <CircuitUnit.h>

class CircuitUnit;
class Sim;

class Line : public CircuitUnit
{
public:
    Line(Sim* sim);

    void prepareVertexData() override;
    void prepareUniforms() override;
    void setupDescriptorSetLayout() override;
    void preparePipeline() override;
    void setupDescriptorSet() override;

    void fillUniforms(int frameNum) override;
    void prepareRender(int frameNum) override;
};