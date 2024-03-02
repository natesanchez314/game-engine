#pragma once

#include "nate_pipeline.hpp"
#include "nate_device.hpp"
#include "nate_game_object.hpp"

#include <memory>
#include <vector>

namespace nate
{
	class SimpleRenderSystem
	{
	private:
		NateDevice& nateDevice;
		VkPipelineLayout pipelineLayout;
		std::unique_ptr<NatePipeline> natePipeline;

	public:
		SimpleRenderSystem(NateDevice& device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem operator=(const SimpleRenderSystem&) = delete;


		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<NateGameObject>& gameObjects);

	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
	};
}