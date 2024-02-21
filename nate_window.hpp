#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace nate
{
	class NateWindow
	{
	private:
		void initWindow();

		const int width;
		const int height;

		std::string windowName;
		GLFWwindow* window;

	public:
		NateWindow(int w, int h, std::string name);
		~NateWindow();

		NateWindow(const NateWindow&) = delete;
		NateWindow& operator=(const NateWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(this->window); }
	};
}