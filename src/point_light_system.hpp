#pragma once

#include "nate_pipeline.hpp"
#include "nate_device.hpp"
#include "nate_game_object.hpp"
#include "nate_camera.hpp"
#include "nate_frame_info.hpp"

#include <memory>
#include <vector>

namespace nate {

	class PointLightSystem {
	private:
		NateDevice& nateDevice;
		VkPipelineLayout pipelineLayout;
		std::unique_ptr<NatePipeline> natePipeline;

	public:
		PointLightSystem(NateDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem operator=(const PointLightSystem&) = delete;


		void render(FrameInfo& frameInfo);

	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
	};
}