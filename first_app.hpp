#pragma once

#include "nate_window.hpp"
#include "nate_pipeline.hpp"
#include "nate_device.hpp"
#include "nate_swap_chain.hpp"

#include <memory>
#include <vector>

namespace nate
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		NateWindow nateWindow{WIDTH, HEIGHT, "Vulkan is hard"};
		NateDevice nateDevice{nateWindow};
		NateSwapChain nateSwapChain{ nateDevice, nateWindow.getExtent() };
		std::unique_ptr<NatePipeline> natePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;

	public:
		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp operator=(const FirstApp&) = delete;

		void run();

	private:
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void drawFrame();
	};
}