#include "OpenGLWindow.h"

#if RENDERER == RENDERER_OPENGL

#endif

OpenGLWindow::OpenGLWindow(int width, int height)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Ingenium", NULL, NULL);

	if (!window) {
		initSuccess = false;
		glfwTerminate();
		return;
	}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		initSuccess = false;
		return;
	}
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

OpenGLWindow::~OpenGLWindow()
{
	glfwTerminate();
}

void OpenGLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}