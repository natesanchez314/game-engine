#include "first_app.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <array>
#include <cassert>

namespace nate
{
	struct SimplePushConstantData
	{
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
	};

	FirstApp::FirstApp()
	{
		loadModels();
		createPipelineLayout();
		recreateSwapChain();
		createCommandBuffers();
	}

	FirstApp::~FirstApp()
	{
		vkDestroyPipelineLayout(nateDevice.device(), pipelineLayout, nullptr);
	}

	void FirstApp::run()
	{
		while (!nateWindow.shouldClose())
		{
			glfwPollEvents();
			drawFrame();
		}

		vkDeviceWaitIdle(nateDevice.device());
	}

	void FirstApp::loadModels()
	{
		std::vector<NateModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		nateModel = std::make_unique<NateModel>(nateDevice, vertices);
	}

	void FirstApp::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(nateDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void FirstApp::createPipeline()
	{
		assert(nateSwapChain != nullptr && "Cannot create pipeline before swap chain!");
		assert(pipelineLayout != nullptr && "Cannot create pipeline before layout!");

		PipelineConfigInfo pipelineConfig{};
		NatePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = nateSwapChain->getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		natePipeline = std::make_unique<NatePipeline>(
			nateDevice,
			"simple_shader.vert.spv",
			"simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApp::recreateSwapChain()
	{
		auto extent = nateWindow.getExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = nateWindow.getExtent();
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(nateDevice.device());
		nateSwapChain = nullptr;
		nateSwapChain = std::make_unique<NateSwapChain>(nateDevice, extent);

		// If render pass compatible do nothing else
		if (nateSwapChain == nullptr)
		{
			nateSwapChain = std::make_unique<NateSwapChain>(nateDevice, extent);
		}
		else
		{
			nateSwapChain = std::make_unique<NateSwapChain>(nateDevice, extent, std::move(nateSwapChain));
			if (nateSwapChain->imageCount() != commandBuffers.size())
			{
				freeCommandBuffers();
				createCommandBuffers();
			}
		}
		createPipeline();
	}

	void FirstApp::createCommandBuffers()
	{
		commandBuffers.resize(nateSwapChain->imageCount());

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

	void FirstApp::freeCommandBuffers()
	{
		vkFreeCommandBuffers(
			nateDevice.device(),
			nateDevice.getCommandPool(),
			static_cast<float>(commandBuffers.size()),
			commandBuffers.data());
		commandBuffers.clear();
	}

	void FirstApp::recordCommandBuffer(int imageIndex)
	{
		static int frame = 0;
		frame = (frame + 1) % 1000;

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = nateSwapChain->getRenderPass();
		renderPassInfo.framebuffer = nateSwapChain->getFrameBuffer(imageIndex);

		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = nateSwapChain->getSwapChainExtent();

		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = { 0.01f, 0.01f, 0.01f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };

		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(nateSwapChain->getSwapChainExtent().width);
		viewport.height = static_cast<float>(nateSwapChain->getSwapChainExtent().height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		VkRect2D scissor{ {0, 0}, nateSwapChain->getSwapChainExtent() };
		vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
		vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

		natePipeline->bind(commandBuffers[imageIndex]);
		nateModel->bind(commandBuffers[imageIndex]);

		for (int j = 0; j < 4; j++)
		{
			SimplePushConstantData push{};
			push.offset = { -0.5f + frame * 0.002f, -0.4f + j * 0.25f };
			push.color = { 0.0f, 0.0f, 0.2f + 0.2f * j };

			vkCmdPushConstants(
				commandBuffers[imageIndex], 
				pipelineLayout, 
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 
				0, 
				sizeof(SimplePushConstantData), 
				&push);

			nateModel->draw(commandBuffers[imageIndex]);
		}

		vkCmdEndRenderPass(commandBuffers[imageIndex]);
		if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to record command buffer!");
		}
	}

	void FirstApp::drawFrame()
	{
		uint32_t imageIndex;
		auto result = nateSwapChain->acquireNextImage(&imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain image!");
		}

		recordCommandBuffer(imageIndex);
		result = nateSwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || nateWindow.wasWindowResized())
		{
			nateWindow.resetWindowResizedFlag();
			recreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to present swap chain image!");
		}

	}
}