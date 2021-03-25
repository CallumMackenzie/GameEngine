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
	Shader* shader;
	Mesh m;
	void onCreate()
	{
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 1600, 900);
		drwn->setClearColour(0x95d57c, 1.f);

		m.loadFromOBJ("D:\\MAND.obj");
		m.scale = { 1, 1, 1 };
		m.rotationCenter = { 1, 1, 1 };
		m.position = { 0, 0, 5 };
		m.setTexture("D:\\Images\\71OpO-3gUfL.bmp");
		for (int i = 0; i < m.tris.size(); i++) {
			m.tris[i].t1 = { 1, 0, 0 };
			m.tris[i].t2 = { 0, 1, 0 };
			m.tris[i].t3 = { 0, 0, 1 };
		}
		m.load();

#if RENDERER == RENDERER_OPENGL
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);

		shader = new Shader("./shaders/3D.vert", "./shaders/def3D.frag");
		shader->use();

		camera.FOV = 80;
		shader->setUniformMatrix4x4("modelViewMatrix", makeTransProjMatrix(m));
		shader->setUniform1I("hasTexture", false);

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

		if (in->getKeyState(32)) {
			m.position.x -= 0.5 * Time::deltaTime;
		}

		//if (in->getKeyState(88))
		//	rotate.z = -0.005;
		//if (in->getKeyState(67))
		//	rotate.z = 0.005;

		camera.rotation = camera.rotation + (rotate * Time::deltaTime);
		camera.position = camera.position + ((foreward * speed) + move) * Time::deltaTime;

		Vector3D rot = { 0.001, 0.002, 0.0013 };
		// m.rotation = m.rotation + (rot * Time::deltaTime);

		refreshProjectionMatrix();
		drwn->beginRender();
		drwn->clear();
#if RENDERER == RENDERER_OPENGL
		shader->setUniformFloat("time", glfwGetTime());
		shader->setUniformMatrix4x4("modelViewMatrix", makeTransProjMatrix(m));
		drwn->peekGLErrors();
#endif
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