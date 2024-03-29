#pragma once

#include "nate_pipeline.hpp"
#include "nate_device.hpp"
#include "game_object.hpp"
#include "camera3d.hpp"
#include "frame_info.hpp"

#include <memory>
#include <vector>

namespace nate {

	class SimpleRenderSystem {
	private:
		NateDevice& nateDevice;
		VkPipelineLayout pipelineLayout;
		std::unique_ptr<NatePipeline> natePipeline;

	public:
		SimpleRenderSystem(NateDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem operator=(const SimpleRenderSystem&) = delete;


		void renderGameObjects(FrameInfo& frameInfo);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
}