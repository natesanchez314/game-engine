#include "nate_window.hpp"

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
}