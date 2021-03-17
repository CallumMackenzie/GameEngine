#pragma once

#include "ModWin.h"
#include "Memory.h"
#include "WindowClass.h"
#include "Window.h"
#include "Log.h"
#include "Direct2D.h"
#include "OpenGL.h"
#include "C_WICImageFactory.h"
#include "LinkedList.h"
#include "Renderable.h"
#include "Shapes.h"
#include "Sprite.h"
#include "Input.h"
#include "Direct2DWindow.h"

struct OpenGLWindow
{
	GLFWwindow* window = nullptr;
	bool initSuccess = true;

	OpenGLWindow(int width, int height);
	~OpenGLWindow();

	void beginRender();
	void endRender();

	void setClearColour(long colour, float alpha);
	void clear();

	int createShader(std::string vertexShader, std::string fragmentShader);

	static unsigned int compileShader(const std::string& src, unsigned int glType);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};