#include <Line.h>
#include <ThinDrawer.h>
#include <Camera.h>
#include <Sim.h>

Line::Line(Sim *sim) : CircuitUnit()
{
    this->unitType = LINE;
    this->sim = sim;
    this->body = new DebugLine(sim->td);
}

void Line::prepareVertexData()
{
    body->prepareVertexData();
}

void Line::prepareUniforms()
{
    body->prepareUniforms();
}

void Line::setupDescriptorSetLayout()
{
    body->setupDescriptorSetLayout();
}

void Line::preparePipeline()
{
    body->preparePipeline();
}

void Line::setupDescriptorSet()
{
    body->setupDescriptorSet();
}

void Line::fillUniforms(int frameNum)
{
    uint8_t* pData;
    s_uboVSLine uboVS;
    uboVS.linePoints = glm::vec4(position.x, position.y, size.x, size.y);
    CHECK_RESULT_VK(vkMapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][0]->memory, 0, sizeof(uboVS), 0, (void**)&pData));
    memcpy(pData, &uboVS, sizeof(uboVS));

    vkUnmapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][0]->memory);

    s_uboMixedLine uboMixed;
    uboMixed.color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f);
    uboMixed.data.x = sim->td->wh->cam->zoom;
    uboMixed.data.y = 0.5f;
    uboMixed.data.z = 0.5f;
    uboMixed.data.w = 0.0f;
    CHECK_RESULT_VK(vkMapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][1]->memory, 0, sizeof(uboMixed), 0, (void**)&pData));
    memcpy(pData, &uboMixed, sizeof(uboMixed));

    vkUnmapMemory(sim->td->logicalDevice, body->uniformBuffers[frameNum][1]->memory);
}

void Line::prepareRender(int frameNum)
{
    VkDeviceSize offsets = 0;
    vkCmdBindPipeline(sim->td->frames[frameNum].commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, body->pipeline);

    vkCmdBindDescriptorSets(sim->td->frames[frameNum].commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            body->pipelineLayout, 0, 1, &body->descriptorSet[frameNum], 0, VK_NULL_HANDLE);

    vkCmdBindVertexBuffers(sim->td->frames[frameNum].commandBuffer, 0, 1, &body->buffers[0]->buffer, &offsets);
    vkCmdBindIndexBuffer(sim->td->frames[frameNum].commandBuffer, body->buffers[1]->buffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(sim->td->frames[frameNum].commandBuffer, body->buffers[1]->count, 1, 0, 0, 1);
}