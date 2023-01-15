#include <ShaderBase.h>
#include <ThinDrawer.h>
#include <SwapChain.h>
#include <vkInit.h>

ShaderBase::ShaderBase(ThinDrawer* thinDrawer)
{
    this->thinDrawer = thinDrawer;
    this->logicalDevice = thinDrawer->logicalDevice;
}

void ShaderBase::prepareVertexData()
{
    const int bufferSize = 2;
    buffers.resize(bufferSize);

    for (int i = 0; i < bufferSize; i++)
    {
        buffers[i] = (s_buffers*)malloc(sizeof(s_buffers));
    }
    s_buffers* vertices = buffers[0];
    s_buffers* indices = buffers[1];

    std::vector<s_basicVertex> vertexBuffer =
            {
                    { {+0.5f, +0.5f} },
                    { {-0.5f, +0.5f} },
                    { {-0.5f, -0.5f} },
                    { {+0.5f, -0.5f} },
            };
    uint32_t vertexBufferSize = static_cast<uint32_t>(vertexBuffer.size()) * sizeof(s_basicVertex);

    std::vector<uint32_t> indexBuffer = { 0, 1, 2, 2, 3, 0 };
    indices->count = static_cast<uint32_t>(indexBuffer.size());
    uint32_t indexBufferSize = indices->count * sizeof(uint32_t);

    thinDrawer->bufferStage(vertexBuffer.data(), vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, vertices);
    thinDrawer->bufferStage(indexBuffer.data(), indexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, indices);
}

void ShaderBase::setupDescriptorSet()
{
    int imCount = thinDrawer->swapChain->imageCount;
    descriptorSet.resize(imCount);

    for (int i = 0; i < imCount; i++)
    {
        VkDescriptorSetAllocateInfo allocInfo = vkinit::descriptorSetAllocateInfo(thinDrawer->descriptorPool, &descriptorSets[0], 1);
        CHECK_RESULT_VK(vkAllocateDescriptorSets(logicalDevice, &allocInfo, &descriptorSet[i]));

        VkWriteDescriptorSet writeDescriptorSet = { };

        writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        writeDescriptorSet.dstSet = descriptorSet[i];
        writeDescriptorSet.descriptorCount = 1;
        writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

        writeDescriptorSet.pBufferInfo = &thinDrawer->sharedOrthoUniform[i]->descriptor;
        writeDescriptorSet.dstBinding = 0;
        vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, VK_NULL_HANDLE);

        for (int j = 0; j < uniformBuffers[i].size(); j++)
        {
            writeDescriptorSet.pBufferInfo = &uniformBuffers[i][j]->descriptor;
            writeDescriptorSet.dstBinding = j + 1;

            vkUpdateDescriptorSets(logicalDevice, 1, &writeDescriptorSet, 0, VK_NULL_HANDLE);
        }
    }
}