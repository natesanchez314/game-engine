#include "first_app.hpp"

#include <stdexcept>

namespace nate
{
	FirstApp::FirstApp()
	{
		createPipelineLayout();
		createPipeline();
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
		}
	}

	void FirstApp::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;
		if (vkCreatePipelineLayout(nateDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline layout!");
		}
	}

	void FirstApp::createPipeline()
	{
		auto pipelineConfig = NatePipeline::defaultPipelineConfigInfo(nateSwapChain.width(), nateSwapChain.height());
		pipelineConfig.renderPass = nateSwapChain.getRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		natePipeline = std::make_unique<NatePipeline>(
			nateDevice,
			"simple_shader.vert.spv",
			"simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApp::createCommandBuffers()
	{

	}

	void FirstApp::drawFrame()
	{

	}
}