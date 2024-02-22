#pragma once

#include <string>
#include <vector>
#include "nate_device.hpp"

namespace nate
{
	struct PipelineConfigInfo
	{

	};

	class NatePipeline
	{
	private:
		NateDevice& nateDevice;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

	public:
		NatePipeline(NateDevice& device, const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);
		~NatePipeline() = default;
		NatePipeline(const NatePipeline&) = delete;
		void operator=(const NatePipeline&) = delete;

		static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath, const PipelineConfigInfo& configInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
	};
}
