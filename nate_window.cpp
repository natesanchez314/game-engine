#include "nate_window.hpp"

#include <stdexcept>

namespace nate
{
	NateWindow::NateWindow(int w, int h, std::string name)
		: width(w), height(h), windowName(name)
	{
		initWindow();
	}

	NateWindow::~NateWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void NateWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	}

	void NateWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}
}