#pragma once

#include "nate_device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace nate 
{
	class NateModel 
	{
	public: 
		struct Vertex 
		{
			glm::vec2 position;
			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

	private:
		NateDevice& nateDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;

	public:
		NateModel(NateDevice& device, const std::vector<Vertex>& vertices);
		~NateModel();

		NateModel(const NateModel&) = delete;
		NateModel operator=(const NateModel&) = delete;

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);
	private:
		void createVertexBuffers(const std::vector<Vertex>& vertices);
	};
}