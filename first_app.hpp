#pragma once

#include "nate_window.hpp"
#include "nate_pipeline.hpp"
#include "nate_device.hpp"
#include "nate_swap_chain.hpp"
#include "nate_model.hpp"
#include "nate_game_object.hpp"

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
		std::unique_ptr<NateSwapChain> nateSwapChain;
		std::unique_ptr<NatePipeline> natePipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers;
		std::vector<NateGameObject> gameObjects;

	public:
		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();
		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);
		void renderGameObjects(VkCommandBuffer commandBuffer);
	};
}