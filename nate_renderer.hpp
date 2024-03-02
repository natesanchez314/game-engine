#pragma once

#include "nate_window.hpp"
#include "nate_device.hpp"
#include "nate_swap_chain.hpp"
#include "nate_model.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace nate
{
	class NateRenderer
	{
	private:
		NateWindow& nateWindow;
		NateDevice& nateDevice;
		std::unique_ptr<NateSwapChain> nateSwapChain;
		std::vector<VkCommandBuffer> commandBuffers;
		uint32_t currentImageIndex{ 0 };
		int currentFrameIndex;
		bool isFrameStarted{ false };

	public:
		NateRenderer(NateWindow& window, NateDevice& device);
		~NateRenderer();

		NateRenderer(const NateRenderer&) = delete;
		NateRenderer operator=(const NateRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const { return nateSwapChain->getRenderPass(); }
		bool isFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer getCurrentCommandBuffer() const 
		{
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress!");
			return commandBuffers[currentFrameIndex]; 
		}

		int getFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress!");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:
		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();
	};
}