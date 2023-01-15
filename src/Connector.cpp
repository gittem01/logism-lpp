#include <Connector.h>
#include <ThinDrawer.h>
#include <Camera.h>
#include <Sim.h>

Connector::Connector(Sim* sim) : CircuitUnit()
{
    this->unitType = DOT;
    this->sim = sim;
    this->body = new DebugCircle(sim->td);
}

void Connector::prepareVertexData()
{
    body->prepareVertexData();
}

void Connector::prepareUniforms()
{
    body->prepareUniforms();
}

void Connector::setupDescriptorSetLayout()
{
    body->setupDescriptorSetLayout();
}

void Connector::preparePipeline()
{
    body->preparePipeline();
}

void Connector::setupDescriptorSet()
{
    body->setupDescriptorSet();
}

void Connector::fillUniforms(int frameNum)
{
    uint8_t* pData;
    s_uboVS uboVS;
    uboVS.modelMatrix = glm::mat4(1.0f);
    uboVS.modelMatrix = glm::translate(uboVS.modelMatrix, glm::vec3(position.x, position.y, 0.0f));
    CHECK_RESULT_VK(vkMapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][0]->memory, 0, sizeof(uboVS), 0, (void**)&pData));
    memcpy(pData, &uboVS, sizeof(uboVS));
    vkUnmapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][0]->memory);

    s_uboFSPoly* uboFSCircle;
    CHECK_RESULT_VK(vkMapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][1]->memory, 0, sizeof(uboFSCircle), 0, (void**)&uboFSCircle));
    uboFSCircle->data.x = sim->td->wh->cam->zoom;

    if (sizeMult > 1.0f)
    {
        uboFSCircle->color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else
    {
        uboFSCircle->data.y = 0.3f;
        uboFSCircle->data.z = 0.9f;
        uboFSCircle->color = glm::vec4(0.0f, 0.4f, 1.0f, 1.0f);
    }

    uboFSCircle->data.w = 1.0f;
    vkUnmapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][1]->memory);

    sizeMult = 1.0f;
}

void Connector::prepareRender(int frameNum)
{
    VkDeviceSize offsets = 0;
    vkCmdBindPipeline(sim->td->frames[frameNum].commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, body->pipeline);

    vkCmdBindDescriptorSets(sim->td->frames[frameNum].commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            body->pipelineLayout, 0, 1, &body->descriptorSet[frameNum], 0, VK_NULL_HANDLE);

    vkCmdBindVertexBuffers(sim->td->frames[frameNum].commandBuffer, 0, 1, &body->buffers[0]->buffer, &offsets);
    vkCmdBindIndexBuffer(sim->td->frames[frameNum].commandBuffer, body->buffers[1]->buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(sim->td->frames[frameNum].commandBuffer, body->buffers[1]->count, 1, 0, 0, 1);
}