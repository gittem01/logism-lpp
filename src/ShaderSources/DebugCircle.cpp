#include <ShaderHeaders/DebugCircle.h>
#include <SwapChain.h>
#include <ThinDrawer.h>
#include <Camera.h>
#include <Shader.h>
#include <definitions.h>
#include <vkInit.h>

void DebugCircle::prepareUniforms()
{
    const int bufferSize = 2;
    int imCount = thinDrawer->swapChain->imageCount;

    uniformBuffers.resize(imCount);

    for (int i = 0; i < imCount; i++)
    {
        uniformBuffers[i].resize(bufferSize);

        for (int j = 0; j < bufferSize; j++)
        {
            uniformBuffers[i][j] = (s_uniformBuffer*)malloc(sizeof(s_uniformBuffer));
        }

        s_uniformBuffer* uniformBufferVS = uniformBuffers[i][0];
        s_uniformBuffer* uniformBufferFS = uniformBuffers[i][1];

        thinDrawer->uniformHelper(sizeof(s_uboVS), uniformBufferVS);

        uint8_t* pData;
        s_uboVS uboVS;
        uboVS.modelMatrix = glm::mat4(1.0f);
        uboVS.modelMatrix = glm::scale(uboVS.modelMatrix, glm::vec3(0.1f, 0.1f, 1.0f));
        CHECK_RESULT_VK(vkMapMemory(logicalDevice, uniformBufferVS->memory, 0, sizeof(uboVS), 0, (void**)&pData));
        memcpy(pData, &uboVS, sizeof(uboVS));
        vkUnmapMemory(logicalDevice, uniformBufferVS->memory);

        thinDrawer->uniformHelper(sizeof(s_uboFSPoly), uniformBufferFS);

        s_uboFSPoly uboFSCircle;
        uboFSCircle.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        uboFSCircle.outerColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        uboFSCircle.data.y = 1.0f;
        uboFSCircle.data.z = 0.5f;
        uboFSCircle.data.w = 1.0f;
        CHECK_RESULT_VK(vkMapMemory(logicalDevice, uniformBufferFS->memory, 0, sizeof(uboFSCircle), 0, (void**)&pData));
        memcpy(pData, &uboFSCircle, sizeof(uboFSCircle));

        vkUnmapMemory(logicalDevice, uniformBufferFS->memory);
    }
}

void DebugCircle::setupDescriptorSetLayout()
{
    const int descriptorSetLayoutCount = 1;

    descriptorSets.resize(descriptorSetLayoutCount);

    VkDescriptorSetLayoutBinding layoutBinding0 =
        vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1, 0);
    VkDescriptorSetLayoutBinding layoutBinding1 =
        vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 1, 1);
    VkDescriptorSetLayoutBinding layoutBinding2 =
        vkinit::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 1, 2);

    VkDescriptorSetLayoutBinding bindings1[] = { layoutBinding0, layoutBinding1, layoutBinding2 };

    VkDescriptorSetLayoutCreateInfo descriptorLayout = vkinit::descriptorSetLayoutCreateInfo(bindings1, sizeof(bindings1) / sizeof(bindings1[0]));
    CHECK_RESULT_VK(vkCreateDescriptorSetLayout(logicalDevice, &descriptorLayout, VK_NULL_HANDLE, &descriptorSets[0]));

    VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = vkinit::pipelineLayoutCreateInfo(descriptorSets.data(), descriptorSets.size());
    CHECK_RESULT_VK(vkCreatePipelineLayout(logicalDevice, &pPipelineLayoutCreateInfo, VK_NULL_HANDLE, &pipelineLayout));
}

void DebugCircle::preparePipeline()
{
    VkGraphicsPipelineCreateInfo* pipelineCreateInfo = thinDrawer->getPipelineInfoBase();
    pipelineCreateInfo->layout = pipelineLayout;

    VkVertexInputBindingDescription vertexInputBinding = { };
    vertexInputBinding.binding = 0;
    vertexInputBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    vertexInputBinding.stride = sizeof(s_basicVertex);

    VkVertexInputAttributeDescription vertexInputAttributeFor0;
    vertexInputAttributeFor0.binding = 0;
    vertexInputAttributeFor0.location = 0;
    vertexInputAttributeFor0.format = VK_FORMAT_R32G32_SFLOAT;
    vertexInputAttributeFor0.offset = 0;

    VkPipelineVertexInputStateCreateInfo vertexInputState = { };
    vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputState.vertexBindingDescriptionCount = 1;
    vertexInputState.vertexAttributeDescriptionCount = 1;
    vertexInputState.pVertexAttributeDescriptions = &vertexInputAttributeFor0;
    vertexInputState.pVertexBindingDescriptions = &vertexInputBinding;

    std::vector<std::string> fileNames = {
            std::string("shaders/DebugCircle/vertex_shader.vert.spv"),
            std::string("shaders/DebugCircle/fragment_shader.frag.spv")
    };

    Shader shader2 = Shader(logicalDevice, fileNames);

    pipelineCreateInfo->stageCount = shader2.shaderStages.size();
    pipelineCreateInfo->pStages = shader2.shaderStages.data();
    pipelineCreateInfo->pVertexInputState = &vertexInputState;

    CHECK_RESULT_VK(vkCreateGraphicsPipelines(logicalDevice, VK_NULL_HANDLE, 1, pipelineCreateInfo, VK_NULL_HANDLE, &pipeline));
}