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
	unsigned int buffer;
	Mesh m;
	void onCreate()
	{
		createWindow("Ingenium", 1600, 900);
		drwn->setClearColour(0x95d57c, 1.f);

		//GLuint vertexArrayID;
		//glGenVertexArrays(1, &vertexArrayID);
		//glBindVertexArray(vertexArrayID);

		m.loadFromOBJ("D:\\cube.obj");
		m.scale = { 1, 1, 1, };
		m.position = { 0, 0, 6, };

		int size = m.tris.size() * 3;
		Vector3D* dat = new Vector3D[size];
		int j = 0;
		for (int i = 0; i < m.tris.size(); i += 3) {
			dat[i] = m.tris[i].p[0];
			dat[i + 1] = m.tris[i].p[1];
			dat[i + 2] = m.tris[i].p[2];
		}

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, m.tris.size() * sizeof(Triangle), dat, GL_DYNAMIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		std::string vShader = getFileAsString("./shaders/2D.vert");
		std::string fShader = getFileAsString("./shaders/def2D.frag");

		unsigned int shader = drwn->createShader(vShader, fShader);
		glUseProgram(shader);

		updateBufferMatrix(m);
		glUniformMatrix4fv(glGetUniformLocation(shader, "transMatrix"), 1, false, &bufferMatrix.m[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader, "projectionMatrix"), 1, false, &projectionMatrix.m[0][0]);


		drwn->clear();
		glDrawArrays(GL_TRIANGLES, 0, (m.tris.size() * 3));
		drwn->endRender();
		drwn->peekGLErrors();
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

		updateBufferMatrix(m);

		drwn->beginRender();
		drwn->clear();
		// renderMesh(m);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glDrawArrays(GL_TRIANGLES, 0, m.tris.size() * 3);
		drwn->peekGLErrors();
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