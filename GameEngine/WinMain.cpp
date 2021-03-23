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
	VertexArray* pts = nullptr;
	unsigned int shader;
	Mesh m;
	void onCreate()
	{
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 900, 900);
		drwn->setClearColour(0x95d57c, 1.f);

		m.loadFromOBJ("D:\\cube.obj");
		m.scale = { 1, 1, 1, };
		m.position = { 0, 0, 10, };

#if RENDERER == RENDERER_OPENGL
		m.toVertexArray(&pts);

		std::string vShader = getFileAsString("./shaders/3D.vert");
		std::string fShader = getFileAsString("./shaders/def3D.frag");

		shader = drwn->createShader(vShader, fShader);
		glUseProgram(shader);

		camera.FOV = 80;
		refreshProjectionMatrix();
		Matrix4x4 mat = makeTransProjMatrix(m);
		glUniformMatrix4fv(glGetUniformLocation(shader, "renderMatrix"), 1, false, &mat.m[0][0]);
		glUniform2f(glGetUniformLocation(shader, "aspectRatio"), drwn->aspectRatio[0], drwn->aspectRatio[1]);
#endif

		drwn->beginRender();
		drwn->clear();
#if RENDERER == RENDERER_OPENGL
		pts->draw();
#endif
		drwn->endRender();
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

		refreshProjectionMatrix();
#if RENDERER == RENDERER_OPENGL
		Matrix4x4 mat = makeTransProjMatrix(m);
		glUniformMatrix4fv(glGetUniformLocation(shader, "renderMatrix"), 1, false, &mat.m[0][0]);
#endif
		drwn->beginRender();
		drwn->clear();
#if RENDERER == RENDERER_DIRECT2D
		renderMeshSimple(m);
#endif
#if RENDERER == RENDERER_OPENGL
		pts->draw();
		drwn->peekGLErrors();
#endif
		drwn->endRender();
	}
};

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Game eg = Game();
	eg.start(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

#ifdef _DEBUG
	printAllocationInfo();
#endif

	return 0;
}