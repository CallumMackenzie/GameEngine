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
	Vector3D positions[10];
	Mesh m;
	void onCreate()
	{
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 1600, 900);
		drwn->setClearColour(0x4b4b4b, 1.f);

		shader = new Shader("./shaders/3D.vert", "./shaders/def3D.frag");

		drwn->peekGLErrors();

		m.loadFromOBJ("D:\\cube.obj");
		m.scale = { 1, 1, 1 };
		m.rotationCenter = { -1, 0, 1 };
		m.position = { 0, 0, 5 };\
		for (int i = 0; i < m.tris.size() / 2; i++) {
			m.tris[i].v[0].t = { 0, 0 };
			m.tris[i].v[1].t = { 0, 1 };
			m.tris[i].v[2].t = { 1, 0 };

			m.tris[i + 1].v[0].t = { 0, 1 };
			m.tris[i + 1].v[1].t = { 1, 1 };
			m.tris[i + 1].v[2].t = { 1, 0 };
		}
		drwn->peekGLErrors();
		m.setTexture("D:\\Images\\71OpO-3gUfL.png");
		drwn->peekGLErrors();
		m.load();

#if RENDERER == RENDERER_OPENGL
		drwn->peekGLErrors();
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		drwn->peekGLErrors();

		shader->use();
		drwn->peekGLErrors();

		camera.FOV = 80;
		shader->setUniformMatrix4x4("modelViewMatrix", makeTransProjMatrix(m));
		shader->setUniform1I("hasTexture", true);
		drwn->peekGLErrors();

		//glUniform1i(glGetUniformLocation(shader, "textureSampler"), 0);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, m.mTexture);
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

		//if (in->getKeyState(88))
		//	rotate.z = -0.005;
		//if (in->getKeyState(67))
		//	rotate.z = 0.005;

		camera.rotation = camera.rotation + (rotate * Time::deltaTime);
		camera.position = camera.position + ((foreward * speed) + move) * Time::deltaTime;

		Vector3D rot = { 0.001, 0.0015, 0.002 };
		m.rotation = m.rotation + (rot * Time::deltaTime);

		refreshProjectionMatrix();
		drwn->beginRender();
		drwn->clear();
		shader->setUniformFloat("time", glfwGetTime());
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