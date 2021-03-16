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

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};