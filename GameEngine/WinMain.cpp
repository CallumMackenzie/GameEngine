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
	Light l;
	void onCreate()
	{
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 1600, 900);
		Time::setFPS(200);
		drwn->setClearColour(0x4b4b4b, 1.f);
		// drwn->setFullScreen(true);

		shader = new Shader("./shaders/3D.vert", "./shaders/def3D.frag");
		shader->use();

		drwn->peekGLErrors();

		m.loadFromOBJ("D:\\mandnormtex.obj", true, true);
		m.scale = { 0.2, 0.2, 0.2 };
		m.rotationCenter = { 0, 0, 0 };
		m.position = { 0, 0, 5 };
		//for (int j = 0; j < m.tris.size(); j++)
		//	for (int i = 0; i < 3; i++)
		//		m.tris[j].v[i].rgb = { 0.5, 0.5, 0.5, 1.0 };
		// m.setTexture("D:\\Cube Map.png");
		m.setTexture("D:\\Images\\Bark_Pine_baseColor.jpg");
		m.load();

		l.colour = { 1, 1, 1, 1 };
		l.intensity = 0.1;
		l.rotation = { 90, 180, 0 };

		camera.FOV = 60;

#if RENDERER == RENDERER_OPENGL
		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);
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
		Vector3D forward = camera.lookVector();
		Vector3D up = { 0, 1, 0 };
		Vector3D move;
		Vector3D rotate;
		if (in->getKeyState(87))
			speed = 0.01;
		if (in->getKeyState(83))
			speed = -0.01;
		if (in->getKeyState(68))
			move = Vector3D::crossProduct(forward, up).normalized() * 0.01;
		if (in->getKeyState(65))
			move = Vector3D::crossProduct(forward, up).normalized() * -0.01;
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

		camera.rotation = camera.rotation + (rotate * Time::deltaTime * 1000);
		camera.position = camera.position + ((forward * speed) + move) * Time::deltaTime * 1000;

		Vector3D rot = { 0.00075, 0.0005, 0.00005 };
		m.rotation = m.rotation + (rot * Time::deltaTime * 1000);

		refreshProjectionMatrix();
		drwn->beginRender();
		drwn->clear();
		shader->setUniformFloat("u_time", glfwGetTime());
		shader->setUniformMatrix4x4("model", m.makeWorldMatrix());
		shader->setUniformMatrix4x4("view", camera.makeCameraMatrix().qInverse());
		shader->setUniformMatrix4x4("projection", projectionMatrix);
		shader->setUniformMatrix4x4("invModel", m.makeWorldMatrix().qInverse());
		l.sendToShader(shader);

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
