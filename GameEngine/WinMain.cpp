#include "Memory.h"

#ifdef _DEBUG
MEM_COUNTER
#endif

#include "Ingenium.h"

using namespace ingenium2D;
using namespace ingenium3D;

// Callum Mackenzie

struct Game : Ingenium3D
{
	//float tris[6] = {
	//-0.5, -0.5,
	//0.0, 0.5,
	//0.5, -0.5
	//};
	//unsigned int buffer;
	Mesh m;
	void onCreate()
	{
		createWindow("Ingenium", 1600, 900);
		drwn->setClearColour(0x95d57c, 1.f);

		GLuint vertexArrayID;
		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		Vector2D p1 = drwn->worldScreenSpaceToScreenSpace(0, 0);
		Vector2D p2 = drwn->worldScreenSpaceToScreenSpace(1600, 0);
		Vector2D p3 = drwn->worldScreenSpaceToScreenSpace(800, 900);

		float tris[6] = { 
			p1.x, p1.y,
			p2.x, p2.y,
			p3.x, p3.y
		};

		glGenBuffers(1, &drwn->buffer);
		glBindBuffer(GL_ARRAY_BUFFER, drwn->buffer);
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), tris, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		std::string vShader = 
			"#version 330 core\n"
			"\n"
			"layout(location = 0) in vec4 position;\n"
			"void main()\n"
			"{\n"
			"	gl_Position = position;\n"
			"}\n";
		std::string fShader =
			"#version 330 core\n"
			"\n"
			"layout(location = 0) out vec4 colour;\n"
			"void main()\n"
			"{\n"
			"colour = vec4(0.0, 0.0, 0.0, 1.0);\n"
			"}\n";

		unsigned int shader = drwn->createShader(vShader, fShader);
		glUseProgram(shader);
		m.loadFromOBJ("D:\\MAND.obj");
		m.scale = 0.3;
		m.position = { 0, 0, 6, 1 };
	};
	void onUpdate() 
	{
		float speed = 0;
		float cameraMoveSpeed = 0.002;
		Vector3D foreward = camera.lookVector();
		Vector3D move;
		Vector3D rotate;
		Input* in = Input::getInput();
		if (in->getKeyState(87))
			speed = 0.01;
		if (in->getKeyState(83))
			speed = -0.01;
		if (in->getKeyState(68))
			move.x = 0.01;
		if (in->getKeyState(65))
			move.x = -0.01;
		if (in->getKeyState(81))
			move.y = 0.01;
		if (in->getKeyState(69))
			move.y = -0.01;

		if (in->getKeyState(37))
			rotate.y = -cameraMoveSpeed;
		if (in->getKeyState(39))
			rotate.y = cameraMoveSpeed;

		if (in->getKeyState(38))
			rotate.x = -cameraMoveSpeed;
		if (in->getKeyState(40))
			rotate.x = cameraMoveSpeed;

		//if (in->getKeyState(88))
		//	rotate.z = -0.005;
		//if (in->getKeyState(67))
		//	rotate.z = 0.005;

		camera.rotation = camera.rotation + (rotate * Time::deltaTime);
		camera.position = camera.position + ((foreward * speed) + move) * Time::deltaTime;

		drwn->beginRender();
		drwn->clear();
		renderMesh(m);
		drwn->endRender();
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Game* eg = new Game();
	eg->start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	delete eg;

#ifdef _DEBUG
	printAllocationInfo();
#endif

	return 0;
}