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
	unsigned int shader;
	Mesh m;
	void onCreate()
	{
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 1600, 900);
		drwn->setClearColour(0x95d57c, 1.f);

		m.loadFromOBJ("D:\\MAND.obj");
		m.scale = { 0.2, 0.2, 0.2 };
		m.position = { 0, 0, 10 };

#if RENDERER == RENDERER_OPENGL
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		std::string vShader = getFileAsString("./shaders/3D.vert");
		std::string fShader = getFileAsString("./shaders/def3D.frag");

		shader = drwn->createShader(vShader, fShader);
		glUseProgram(shader);

		camera.FOV = 80;
		refreshProjectionMatrix();
		Matrix4x4 mat = makeTransProjMatrix(m);
		glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, false, &mat.m[0][0]);
		glUniform2f(glGetUniformLocation(shader, "aspectRatio"), drwn->aspectRatio[0], drwn->aspectRatio[1]);

		Debug::oss << "Vec2D: " << sizeof(Vector2D) << "   float: " << sizeof(float) << "   Vec3D: " << sizeof(Vector3D);
		Debug::writeLn();
#endif
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

		if (in->getKeyState(32)) {
			m.position.x -= 0.5 * Time::deltaTime;
		}

		//if (in->getKeyState(88))
		//	rotate.z = -0.005;
		//if (in->getKeyState(67))
		//	rotate.z = 0.005;

		camera.rotation = camera.rotation + (rotate * Time::deltaTime);
		camera.position = camera.position + ((foreward * speed) + move) * Time::deltaTime;

		refreshProjectionMatrix();
		drwn->beginRender();
		drwn->clear();
#if RENDERER == RENDERER_OPENGL
		Matrix4x4 mat = makeTransProjMatrix(m);
		glUniformMatrix4fv(glGetUniformLocation(shader, "modelViewMatrix"), 1, false, &mat.m[0][0]);
		drwn->peekGLErrors();
#endif
		renderMeshSimple(m);
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