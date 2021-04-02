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
	Light* llst = new Light[2];
	DirectionalLight dirLight;

	void onCreate()
	{
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 1600, 900);
		glfwSetWindowPos(drwn->window, 100, 100);
		Time::setFPS(200);
		drwn->setClearColour(0x4b4b4b, 1.f);
		// drwn->setFullScreen(true);

		shader = new Shader("./shaders/3D.vert", "./shaders/def3D.frag");
		shader->use();

		drwn->peekGLErrors();

		skyBox.loadFromOBJ("D:\\planenormtex.obj", true, true);
		skyBox.scale = { 10, 1, 10 };
		skyBox.setTexture("D:\Images\\tex.jpg");
		skyBox.material.shininess = 1;
		skyBox.load();
		skyBox.position = { -3, 0, 0 };

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
		// lightObj.setTexture("D:\\Images\\Bark_Pine_normal.jpg", "D:\\Images\\Ground_Forest_002_normal.jpg");
		lightObj.setTexture("NONE", "D:\\Images\\Ground_Forest_002_normal.jpg");
		lightObj.load();
		lightObj.position = { 9, 3, 4 };
		lightObj.scale = { 1, 1, 1 };
		lightObj.material.shininess = 1;

		llst[0].diffuse = { 1, 1, 1 };
		llst[0].ambient = { 0.2, 0.2, 0.2 };
		llst[0].specular = { 0.6, 0.6, 0.6 };

		llst[1].diffuse = { 0.6, 0.6, 0.6 };
		llst[1].specular = { 0.8, 0.8, 0.8 };
		llst[1].ambient = { 0.01, 0.01, 0.01 };

		dirLight.ambient = { 0.001, 0.001, 0.001 };
		dirLight.diffuse = { 0.01, 0.01, 0.01 };
		dirLight.specular = { 0.01, 0.01, 0.01 };
		dirLight.position = { 0, -1, 0.5 };

		camera.FOV = 60;
		camera.clipNear = 0.2;
		camera.clipFar = 500;
		refreshProjectionMatrix();

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LEQUAL);
		glDepthRange(0.0f, 1.0f);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_MULTISAMPLE);

		glfwSetInputMode(drwn->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(drwn->window, mouse_callback);
	};
	void onUpdate()
	{
		Input* in = Input::getInput(drwn->window);
		//Debug::oss << "FPS: " << 1.f / Time::deltaTime;
		//Debug::writeLn();
		float speed = 3;
		float cameraMoveSpeed = 0.002;
		Vector3D cLV = camera.lookVector();

		Vector3D forward;
		Vector3D up = { 0, 1, 0 };
		Vector3D rotate;
		if (in->getKeyState(87))
			forward = forward + cLV;
		if (in->getKeyState(83))
			forward = forward + cLV * -1;
		if (in->getKeyState(68))
			forward = forward + Vector3D::crossProduct(cLV, up);
		if (in->getKeyState(65))
			forward = forward + Vector3D::crossProduct(cLV, up) * -1;
		if (in->getKeyState(81) || in->getKeyState(32))
			forward.y = forward.y + 1;
		if (in->getKeyState(69))
			forward.y = forward.y - 1;

		float cSpeed = 4;
		if (in->getKeyState(37) || in->getKeyState(GLFW_KEY_LEFT))
			cube.position.x += cSpeed * Time::deltaTime;
		if (in->getKeyState(39) || in->getKeyState(GLFW_KEY_RIGHT))
			cube.position.x -= cSpeed * Time::deltaTime;

		if (in->getKeyState(38) || in->getKeyState(GLFW_KEY_UP))
			cube.position.y += cSpeed * Time::deltaTime;
		if (in->getKeyState(40) || in->getKeyState(GLFW_KEY_DOWN))
			cube.position.y -= cSpeed * Time::deltaTime;

		if (in->getKeyState(340) || in->getKeyState(16))
			speed *= 5;

		camera.rotation = camera.rotation + (rotate * Time::deltaTime * 1000);
		camera.position = camera.position + forward.normalized() * speed * Time::deltaTime;
		
		cameraCorrection();

		cube.rotation = cube.rotation + (Vector3D{ 0.5, 0.5, 0.5 } * Time::deltaTime);
		lightObj.rotation = lightObj.rotation + (Vector3D{ 5, 0.7, 3 } * Time::deltaTime);
		llst[1].position = cube.position;

		llst[0].position = camera.position + Vector3D{ 0, 1.9, 0 };

		drwn->beginRender();
		drwn->clear();
		Light::sendLightsToShader(shader, llst, 2);
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

	void cameraCorrection() {
		if (abs(camera.rotation.x) > 1.527163)
			camera.rotation.x = 1.527163 * utils3d::sign(camera.rotation.x);
		if (abs(camera.rotation.y) > 6.28319)
			camera.rotation.y = 0;
	}

	static inline float lastX = 800, lastY = 450;
	static inline const float sensitivity = 0.3f;

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		xoffset *= sensitivity;
		yoffset *= sensitivity;
		engine3D->camera.rotation.y += xoffset * Time::deltaTime;
		engine3D->camera.rotation.x -= yoffset * Time::deltaTime;
		((Game*) engine3D)->cameraCorrection();
	};
};

//int main() {
//	Game eg = Game();
//	eg.start();
//
//#ifdef _DEBUG
//	printAllocationInfo();
//#endif
//	return 0;
//}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	Game eg = Game();
	eg.start();

#ifdef _DEBUG
	printAllocationInfo();
#endif

	return 0;
}
