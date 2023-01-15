#include <ShaderHeaders/DebugCircle.h>
#include <glm/glm.hpp>
#include <CircuitUnit.h>

class CircuitUnit;
class Sim;

class Connector : public CircuitUnit
{
public:
    Connector(Sim* sim);

    float sizeMult = 1.0f;

    void prepareVertexData() override;
    void prepareUniforms() override;
    void setupDescriptorSetLayout() override;
    void preparePipeline() override;
    void setupDescriptorSet() override;

    void fillUniforms(int frameNum) override;
    void prepareRender(int frameNum) override;
};