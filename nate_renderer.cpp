#include "nate_renderer.hpp"

#include <stdexcept>
#include <array>
#include <cassert>

namespace nate
{
	NateRenderer::NateRenderer(NateWindow& window, NateDevice& device)
		:nateWindow(window), nateDevice(device)
	{
		recreateSwapChain();
		createCommandBuffers();
	}

	NateRenderer::~NateRenderer()
	{
		freeCommandBuffers();
	}

	void NateRenderer::recreateSwapChain()
	{
		auto extent = nateWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = nateWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(nateDevice.device());

		if (nateSwapChain == nullptr)
		{
			nateSwapChain = std::make_unique<NateSwapChain>(nateDevice, extent);
		}
		else
		{
			std::shared_ptr<NateSwapChain> oldSwapChain = std::move(nateSwapChain);
			nateSwapChain = std::make_unique<NateSwapChain>(nateDevice, extent, oldSwapChain);
			if (!oldSwapChain->compareSwapFormats(*nateSwapChain.get()))
			{
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}
		}
	}

	void NateRenderer::createCommandBuffers()
	{
		commandBuffers.resize(NateSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = nateDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(nateDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw::std::runtime_error("Failed to allocate command buffers!");
		}
	}

	void NateRenderer::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			nateDevice.device(),
			nateDevice.getCommandPool(),
			static_cast<float>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	VkCommandBuffer NateRenderer::beginFrame()
	{
		assert(!isFrameStarted && "Can't call beginFrame while already in progress!");

		auto result = nateSwapChain->acquireNextImage(&currentImageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return nullptr;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		isFrameStarted = true;

		auto commandBuffer = getCurrentCommandBuffer();

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		return commandBuffer;
	}

	void NateRenderer::endFrame()
	{
		assert(isFrameStarted && "Can't call endFrame if frame is not in progress!");

		auto commandBuffer = getCurrentCommandBuffer();

		if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}

		auto result = nateSwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || nateWindow.wasWindowResized())
		{
			nateWindow.resetWindowResizedFlag();
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image!");
		}

		isFrameStarted = false;
		currentFrameIndex = (currentFrameIndex + 1) % NateSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void NateRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command bufer from a different frame!");

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = nateSwapChain->getRenderPass();
		renderPassInfo.framebuffer = nateSwapChain->getFrameBuffer(currentImageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = nateSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(nateSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(nateSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, nateSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
	}

	void NateRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
	{
		assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress!");
		assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command bufer from a different frame!");

		vkCmdEndRenderPass(commandBuffer);
	}
}