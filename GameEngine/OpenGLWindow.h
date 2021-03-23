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
#include "3DData.h"
#include "RenderWindow.h"

struct OpenGLWindow : RenderWindow
{
	GLFWwindow* window = nullptr;
	bool initSuccess = true;
	unsigned int buffer;

	Vector2D scale = { 1, 1 };
	Vector2D translation;

	OpenGLWindow(int width, int height);
	~OpenGLWindow();

	void beginRender();
	void endRender();

	float screenWidth();
	float screenHeight();

	void drawTriangle(float point1X, float point1Y, float point2X, float point2Y, float point3X, float point3Y);

	Vector2D worldScreenSpaceToScreenSpace(float wX, float wY);
	void peekGLErrors();

	void setClearColour(long colour, float alpha);
	void clear();

	int createShader(std::string vertexShader, std::string fragmentShader);

	static unsigned int compileShader(const std::string& src, unsigned int glType);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	template< typename T >
	static inline std::string intToHex(T i)
	{
		std::stringstream stream;
		stream << "0x" << std::hex << i;
		return stream.str();
	}
};