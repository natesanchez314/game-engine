#pragma once

#include "nate_window.hpp"

namespace nate
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

	private:
		NateWindow nateWindow{WIDTH, HEIGHT, "Vulkan is hard"};

	public:
		void run();
	};
}