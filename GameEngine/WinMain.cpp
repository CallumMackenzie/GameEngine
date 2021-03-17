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

		//GLuint vertexArrayID;
		//glGenVertexArrays(1, &vertexArrayID);
		//glBindVertexArray(vertexArrayID);

		//glGenBuffers(1, &buffer);
		//glBindBuffer(GL_ARRAY_BUFFER, buffer);
		//glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), tris, GL_STATIC_DRAW);

		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

		//std::string vShader = 
		//	"#version 330 core\n"
		//	"\n"
		//	"layout(location = 0) in vec4 position;\n"
		//	"void main()\n"
		//	"{\n"
		//	"	gl_Position = position;\n"
		//	"}\n";
		//std::string fShader =
		//	"#version 330 core\n"
		//	"\n"
		//	"layout(location = 0) out vec4 colour;\n"
		//	"void main()\n"
		//	"{\n"
		//	"colour = vec4(0.0, 0.0, 0.0, 1.0);\n"
		//	"}\n";

		//unsigned int shader = drwn->createShader(vShader, fShader);
		//glUseProgram(shader);
		m.loadFromOBJ("D:\\cube.obj");
	};
	void onUpdate() 
	{
		drwn->beginRender();
		drwn->clear();
		//glDrawArrays(GL_TRIANGLES, 0, 3);
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