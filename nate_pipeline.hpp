#pragma once

#include <string>
#include <vector>

namespace nate
{
	class NatePipeline
	{
	public:
		NatePipeline(const std::string& vertFilePath, const std::string& fragFilePath);

	private:
		static std::vector<char> readFile(const std::string& filePath);

		void createGraphicsPipeline(const std::string& vertFilePath, const std::string& fragFilePath);
	};
}
