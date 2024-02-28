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
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizedCallback);
	}

	void NateWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void NateWindow::frameBufferResizedCallback(GLFWwindow* window, int width, int height)
	{
		auto nateWindow = reinterpret_cast<NateWindow*>(glfwGetWindowUserPointer(window));
		nateWindow->frameBufferResized = true;
		nateWindow->width = width;
		nateWindow->height = height;
	}
}