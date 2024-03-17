#pragma once

#include "nate_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace nate {

    class NateDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(NateDevice& nateDevice) : nateDevice{ nateDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<NateDescriptorSetLayout> build() const;

        private:
            NateDevice& nateDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        NateDescriptorSetLayout(NateDevice& nateDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~NateDescriptorSetLayout();
        NateDescriptorSetLayout(const NateDescriptorSetLayout&) = delete;
        NateDescriptorSetLayout& operator=(const NateDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        NateDevice& nateDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class NateDescriptorWriter;
    };

    class NateDescriptorPool {
    public:
        class Builder {
        public:
            Builder(NateDevice& nateDevice) : nateDevice{ nateDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<NateDescriptorPool> build() const;

        private:
            NateDevice& nateDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        NateDescriptorPool(
            NateDevice& nateDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~NateDescriptorPool();
        NateDescriptorPool(const NateDescriptorPool&) = delete;
        NateDescriptorPool& operator=(const NateDescriptorPool&) = delete;

        bool allocateDescriptorSet(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        NateDevice& nateDevice;
        VkDescriptorPool descriptorPool;

        friend class NateDescriptorWriter;
    };

    class NateDescriptorWriter {
    public:
        NateDescriptorWriter(NateDescriptorSetLayout& setLayout, NateDescriptorPool& pool);

        NateDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        NateDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        NateDescriptorSetLayout& setLayout;
        NateDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace Nate