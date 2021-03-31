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
	Mesh skyBox;
	Mesh m;
	Mesh cube;
	Mesh lightObj;
	Light* llst = new Light[1];
	DirectionalLight dirLight;
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

		skyBox.loadFromOBJ("D:\\cubenormaltex.obj", true, true);
		skyBox.scale = { 10, 10, 10 };
		skyBox.setTexture("D:\\Images\\image.jpg");
		skyBox.load();
		skyBox.position = Vector3D{ -5, 0, 0 };

		m.loadFromOBJ("D:\\mandnormtex.obj", true, true);
		m.scale = { 0.2, 0.2, 0.2 };
		m.position = { 0, 0, 5 };
		m.material.shininess = 0.5;
		m.setTexture("D:\\Images\\CrackSoil.jpg", "D:\\Images\\CrackSoilGrayScale.jpg");
		m.load();
		
		cube.loadFromOBJ("D:\\cubenormaltex.obj", true, true);
		cube.setTexture("D:\\Images\\tex.jpg", "D:\\Images\\Ground_Forest_002_roughness.jpg");
		cube.load();
		cube.material.shininess = 0.5;
		cube.position = { 1, 10, 0 };
		cube.scale = { 0.5, 0.5, 0.5 };

		lightObj.loadFromOBJ("D:\\spheretexnorm.obj", true, true);
		lightObj.setTexture("D:\\Images\\Bark_Pine_normal.jpg", "D:\\Images\\Ground_Forest_002_normal.jpg");
		lightObj.load();
		lightObj.position = { 9, 3, 4 };
		lightObj.scale = { 0.5, 0.5, 0.5 };
		lightObj.material.shininess = 0;

		//llst[0].diffuse = { 0, 0, 0 };
		//llst[0].ambient = { 0, 0, 0 };
		//llst[0].specular = { 0, 0, 0 };
		//llst[0].intensity = 1;

		dirLight.ambient = { 0.001, 0.001, 0.001 };
		dirLight.diffuse = { 0.01, 0.01, 0.01 };
		dirLight.specular = { 0.01, 0.01, 0.01 };
		dirLight.intensity = 1;
		dirLight.position = { 0, -1, 0 };

		camera.FOV = 60;
		camera.clipNear = 0.1;
		camera.clipFar = 100;
		refreshProjectionMatrix();

		glDisable(GL_CULL_FACE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);
	};
	void onUpdate()
	{
		Input* in = Input::getInput();
		if (in->getKeyState(32))
			return;
		//Debug::oss << "FPS: " << 1.f / Time::deltaTime;
		//Debug::writeLn();
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

		if (in->getKeyState(16)) {
			move = move * 3;
			speed *= 3;
		}

		camera.rotation = camera.rotation + (rotate * Time::deltaTime * 1000);
		camera.position = camera.position + ((forward * speed) + move) * Time::deltaTime * 1000;

		if (abs(camera.rotation.x) > 1.527163)
			camera.rotation.x = 1.527163 * utils3d::sign(camera.rotation.x);
		if (abs(camera.rotation.y) > 6.28319)
			camera.rotation.y = 0;

		cube.rotation = cube.rotation + (Vector3D{ 0.5, 0.5, 0.5 } * Time::deltaTime);
		lightObj.rotation = lightObj.rotation + (Vector3D{ 5, 0.7, 3 } * Time::deltaTime);

		llst[0].position = camera.position + Vector3D{ 0, 1.9, 0 };

		drwn->beginRender();
		drwn->clear();
		Light::sendLightsToShader(shader, llst, 1);
		dirLight.sendToShader(shader);

		m.render(shader, camera, &projectionMatrix);
		cube.render(shader, camera, &projectionMatrix);
		lightObj.render(shader, camera, &projectionMatrix);
		skyBox.render(shader, camera, &projectionMatrix);

		drwn->endRender();
	}
	void onClose() {
		memory::safe_delete(shader);
		memory::safe_delete(llst);
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
