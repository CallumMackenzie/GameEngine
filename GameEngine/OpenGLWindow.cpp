#include "OpenGLWindow.h"

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
		Debug::oss << "Failed to create GLFW window.";
		Debug::writeLn();
		return;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		initSuccess = false;
		Debug::oss << "Failed to initialize GLAD.";
		Debug::writeLn();
		return;
	}
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

OpenGLWindow::~OpenGLWindow()
{
	glfwTerminate();
}

void OpenGLWindow::beginRender()
{
	
}

void OpenGLWindow::endRender()
{

	glfwSwapBuffers(window);
}

void OpenGLWindow::setClearColour(long colour, float alpha)
{
	int r = (colour & 0xFF0000) >> 16;
	int g = (colour & 0x00FF00) >> 8;
	int b = (colour & 0x0000FF);
	glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, alpha);
}

void OpenGLWindow::clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}


int OpenGLWindow::createShader(std::string vertexShader, std::string fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(vertexShader, GL_VERTEX_SHADER);
	unsigned int fs = compileShader(fragmentShader, GL_FRAGMENT_SHADER);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	return program;
}

unsigned int OpenGLWindow::compileShader(const std::string& src, unsigned int glType)
{
	unsigned int id = glCreateShader(glType);
	const char* srcCStr = src.c_str();
	glShaderSource(id, 1, &srcCStr, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int eLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &eLength);
		char* message = (char*) alloca(eLength * sizeof(char));
		glGetShaderInfoLog(id, eLength, &eLength, message);
		Debug::oss << "Failed to compile " << (glType == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << message;
		Debug::writeLn();
		glDeleteShader(id);
	}

	return id;
}

void OpenGLWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}