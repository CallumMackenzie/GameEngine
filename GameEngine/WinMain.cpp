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
	Shader* shader = nullptr;
	Mesh m;
	void onCreate()
	{
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 1600, 900);
		Time::setFPS(200);
		drwn->setClearColour(0x4b4b4b, 1.f);
		// drwn->setFullScreen(true);

		shader = new Shader("./shaders/3D.vert", "./shaders/def3D.frag");

		drwn->peekGLErrors();

		m.loadFromOBJ("D:\\cubemapped.obj", true);
		m.scale = { 1, 1, 1 };
		m.rotationCenter = { 0, 0, 0 };
		m.position = { 0, 0, 5 };
		m.setTexture("D:\\Cube Map.png");
		m.load();

#if RENDERER == RENDERER_OPENGL
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		shader->use();

		camera.FOV = 60;
		shader->setUniformMatrix4x4("modelViewMatrix", makeTransProjMatrix(m));
		drwn->peekGLErrors();

		//glUniform1i(glGetUniformLocation(shader, "textureSampler"), 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m.mTexture);
#endif
	};
	void onUpdate()
	{
		Input* in = Input::getInput();
		if (in->getKeyState(32))
			return;
		Debug::oss << "FPS: " << 1.f / Time::deltaTime;
		Debug::writeLn();
		float speed = 0;
		float cameraMoveSpeed = 0.002;
		Vector3D foreward = camera.lookVector();
		Vector3D up = { 0, 1, 0 };
		Vector3D move;
		Vector3D rotate;
		if (in->getKeyState(87))
			speed = 0.01;
		if (in->getKeyState(83))
			speed = -0.01;
		if (in->getKeyState(68))
			move = Vector3D::crossProduct(foreward, up).normalized() * 0.01;
		if (in->getKeyState(65))
			move = Vector3D::crossProduct(foreward, up).normalized() * -0.01;
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

		camera.rotation = camera.rotation + (rotate * Time::deltaTime * 1000);
		camera.position = camera.position + ((foreward * speed) + move) * Time::deltaTime * 1000;

		Vector3D rot = { 0.001, 0.000, 0.001 };
		m.rotation = m.rotation + (rot * Time::deltaTime * 1000);

		refreshProjectionMatrix();
		drwn->beginRender();
		drwn->clear();
		shader->setUniformFloat("u_time", glfwGetTime());
		shader->setUniformMatrix4x4("modelViewMatrix", makeTransProjMatrix(m));
		drwn->peekGLErrors();
		renderMeshSimple(m);
		drwn->endRender();
	}
	void onClose() {
		memory::safe_delete(shader);
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