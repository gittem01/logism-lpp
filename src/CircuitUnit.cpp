#include <ThinDrawer.h>
#include <Sim.h>
#include <Camera.h>
#include <Connector.h>

CircuitUnit::CircuitUnit(Sim* sim)
{
    this->sim = sim;
    this->body = (ShaderBase*)(new DebugPolygon(sim->td));

    this->connectors.push_back(new Connector(sim));
    this->connectors.at(this->connectors.size() - 1)->position = {14, 1};

    this->connectors.push_back(new Connector(sim));
    this->connectors.at(this->connectors.size() - 1)->position = {14, 2};

    this->connectors.push_back(new Connector(sim));
    this->connectors.at(this->connectors.size() - 1)->position = {14, 3};

    this->connectors.push_back(new Connector(sim));
    this->connectors.at(this->connectors.size() - 1)->position = {1, 0};

    this->connectors.push_back(new Connector(sim));
    this->connectors.at(this->connectors.size() - 1)->position = {4, 0};

    this->connectors.push_back(new Connector(sim));
    this->connectors.at(this->connectors.size() - 1)->position = {6, 0};

    this->position = {0, 0};
    this->size = {14, 9};

    for (int x = 0; x < this->size.x; x++)
    {
        for (int y = 0; y < this->size.y; y++)
        {
            sim->fillGrid(this, this->position, this->size);
        }
    }
}

CircuitUnit::CircuitUnit()
{

}

void CircuitUnit::prepareVertexData()
{
    body->prepareVertexData();
    for (Connector* cntr : connectors)
    {
        cntr->prepareVertexData();
    }
}

void CircuitUnit::prepareUniforms()
{
    body->prepareUniforms();
    for (Connector* cntr : connectors)
    {
        cntr->prepareUniforms();
    }
}

void CircuitUnit::setupDescriptorSetLayout()
{
    body->setupDescriptorSetLayout();
    for (Connector* cntr : connectors)
    {
        cntr->setupDescriptorSetLayout();
    }
}

void CircuitUnit::preparePipeline()
{
    body->preparePipeline();
    for (Connector* cntr : connectors)
    {
        cntr->preparePipeline();
    }
}

void CircuitUnit::setupDescriptorSet()
{
    body->setupDescriptorSet();
    for (Connector* cntr : connectors)
    {
        cntr->setupDescriptorSet();
    }
}

void CircuitUnit::fillUniforms(int frameNum)
{
    s_uboVSPOLY* polyData;
    CHECK_RESULT_VK(vkMapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][0]->memory, 0, sizeof(polyData), 0, (void**)&polyData));
    polyData->modelMatrix = glm::mat4(1.0f);
    polyData->modelMatrix = glm::translate(polyData->modelMatrix, glm::vec3(position.x, position.y, 0.0f));
    polyData->polyPoints[0] = glm::vec4(0.0f, 0.0f, size.x, 0.0f);
    polyData->polyPoints[1] = glm::vec4(size.x, size.y, 0.0f, size.y);
    polyData->numPoints = 4;
    vkUnmapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][0]->memory);

    s_uboFSPoly* uboPoly;
    CHECK_RESULT_VK(vkMapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][1]->memory, 0, sizeof(uboPoly), 0, (void**)&uboPoly));
    uboPoly->data.x = sim->td->wh->cam->zoom;
    uboPoly->data.y = 0.3f;
    uboPoly->color = glm::vec4(1.0f, 1.0f, 1.0f, 0.2f);
    uboPoly->outerColor = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
    vkUnmapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][1]->memory);

    for (Connector* cntr : connectors)
    {
        cntr->fillUniforms(frameNum);
        sim->dotGrid[cntr->position.y + Sim::midPosY][cntr->position.x + Sim::midPosX] = (CircuitUnit*)cntr;
    }
}

void CircuitUnit::prepareRender(int frameNum)
{
    VkDeviceSize offsets = 0;
    vkCmdBindPipeline(sim->td->frames[frameNum].commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, body->pipeline);

    vkCmdBindDescriptorSets(sim->td->frames[frameNum].commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            body->pipelineLayout, 0, 1, &body->descriptorSet[frameNum], 0, VK_NULL_HANDLE);

    vkCmdDraw(sim->td->frames[frameNum].commandBuffer, 6, 1, 0, 0);

    for (Connector* cntr : connectors)
    {
        cntr->prepareRender(frameNum);
    }
}