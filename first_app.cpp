#include "first_app.hpp"

namespace nate
{
	void FirstApp::run()
	{
		while (!nateWindow.shouldClose())
		{
			glfwPollEvents();
		}
	}
}