#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <vector>

typedef struct
{
    VkDeviceMemory memory;
    VkBuffer buffer;
    uint32_t count;
} s_buffers;

typedef struct
{
    VkDeviceMemory memory;
    VkBuffer buffer;
    VkDescriptorBufferInfo descriptor;
} s_uniformBuffer;

typedef struct
{
    VkSampler sampler;
    VkImage image;
    VkImageLayout imageLayout;
    VkDeviceMemory deviceMemory;
    VkDescriptorSet set;
    VkImageView view;
    uint32_t width, height;
    uint32_t mipLevels;
} s_texture;

class ThinDrawer;
class CircuitUnit;
class Connector;
class Sim;

class ShaderBase
{
    friend class ThinDrawer;
    friend class CircuitUnit;
    friend class Sim;
    friend class Connector;
    friend class Line;

protected:
    std::vector<VkDescriptorSetLayout> descriptorSets;
    std::vector<s_texture*> textureData;
    VkPipelineLayout pipelineLayout;
    VkPipeline pipeline;
    std::vector<VkDescriptorSet> descriptorSet;

    std::vector<s_buffers*> buffers;
    std::vector<std::vector<s_uniformBuffer*>> uniformBuffers;

    ThinDrawer* thinDrawer;
    VkDevice logicalDevice;

    ShaderBase(ThinDrawer* thinDrawer);

    virtual void prepareVertexData();
    virtual void prepareUniforms() = 0;
    virtual void setupDescriptorSetLayout() = 0;
    virtual void preparePipeline() = 0;
    virtual void setupDescriptorSet();
};