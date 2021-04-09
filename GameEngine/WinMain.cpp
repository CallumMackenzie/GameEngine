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
	std::vector<Mesh> m;
	DirectionalLight dirLight;

	void onCreate()
	{
		// Load times of 50 uv spheres with 3 textures:
		// No caching:					4642 ms
		// Just texture ref caching:	1174 ms
		// Just geometry ref caching:	3608 ms
		// Just geometry val caching:	3599 ms
		// Geometry ref & texture ref:	117  ms
		// Geometry val & texture ref:	112  ms
		// All caching enabled:			113  ms

		Mesh::textureReferenceCache.use = true;
		Mesh::geometryReferenceCache.use = true;
		Mesh::geometryValueCache.use = true;
		engine3D = this;
		engine = this;
		createWindow("Ingenium", 1600, 900);
		glfwSetWindowPos(drwn->window, 100, 100);
		Time::setFPS(200);
		drwn->setClearColour(0x4b4b4b, 1.f);
		// drwn->setFullScreen(true);

		shader = new Shader("./shaders/vert3d.vert", "./shaders/phong.frag");
		shader->use();

		drwn->peekGLErrors();

		auto objPath = "./resource/uvsmoothnt.obj";

		auto startClock = clock();
		for (int i = 0; i < 1000; i++) {
			m.push_back(Mesh{ (float)i * 2.f, 0, 0 });
			m[i].material.shininess = 0.3;
			m[i].useGeometryValueCache = false;
			m[i].make(objPath, "./resource/metal/b.jpg", "./resource/metal/s.jpg", "./resource/metal/n.jpg");
		}
		Debug::oss << "Full load time: " << (((float)(clock() - startClock) / (float)CLOCKS_PER_SEC) * 1000.f) << "ms";
		Debug::writeLn();

		//m.push_back(Mesh{ -1.5, 0, 3 });
		//m[0].material.shininess = 0.4;
		//m[0].make(objPath, "./resource/sbrick/b.jpg", "NONE", "./resource/sbrick/n.jpg");

		//m.push_back(Mesh{ 1.5, 0, 3 });
		//m[1].material.shininess = 0.1;
		//m[1].make(objPath, "./resource/metal/b.jpg", "./resource/metal/s.jpg", "./resource/metal/n.jpg");

		//m.push_back(Mesh{ -4.5, 0, 3 });
		//m[2].material.shininess = 0.2;
		//m[2].make(objPath, "./resource/paper/b.jpg", "NONE", "./resource/paper/n.jpg");

		//m.push_back(Mesh{ 4.5, 0, 3 });
		//m[3].material.shininess = 50;
		//m[3].make(objPath, "./resource/scrmetal/b.jpg", "./resource/scrmetal/s.jpg", "./resource/scrmetal/n.jpg");

		//m.push_back(Mesh{ -1.5, 3, 3 });
		//m[4].material.shininess = 1;
		//m[4].make(objPath, "./resource/gate/b.jpg", "./resource/gate/s.jpg", "./resource/gate/n.jpg");

		//m.push_back(Mesh{ 1.5, 3, 3 });
		//m[5].material.shininess = 0.6;
		//m[5].make(objPath, "./resource/mtrim/b.jpg", "./resource/mtrim/s.jpg", "./resource/mtrim/n.jpg");

		//m.push_back(Mesh{ 4.5, 3, 3 });
		//m[6].material.shininess = 10;
		//m[6].make(objPath, "./resource/woodp/b.jpg", "NONE", "./resource/woodp/n.jpg");

		//m.push_back(Mesh{ -4.5, 3, 3 });
		//m[7].material.shininess = 0.4;
		//m[7].make(objPath, "./resource/mplate/b.jpg", "./resource/mplate/s.jpg", "./resource/mplate/n.jpg");

		dirLight.ambient = { 0.001, 0.001, 0.001 };
		dirLight.diffuse = { 0.5, 0.5, 0.5 };
		dirLight.specular = { 0.4, 0.4, 0.4 };
		dirLight.position = { 0.2, -1, 0.2 };

		camera.FOV = 60;
		camera.clipNear = 0.2;
		camera.clipFar = 500;
		refreshProjectionMatrix();

		// glEnable(GL_CULL_FACE);
		// glCullFace(GL_BACK);
		glDisable(GL_CULL_FACE);
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
		Vec3 cLV = camera.lookVector();

		Vec3 forward;
		Vec3 up = { 0, 1, 0 };
		Vec3 rotate;
		if (in->getKeyState(87))
			forward = forward + cLV;
		if (in->getKeyState(83))
			forward = forward + cLV * -1;
		if (in->getKeyState(68))
			forward = forward + Vec3::crossProduct(cLV, up);
		if (in->getKeyState(65))
			forward = forward + Vec3::crossProduct(cLV, up) * -1;
		if (in->getKeyState(81) || in->getKeyState(32))
			forward.y = forward.y + 1;
		if (in->getKeyState(69))
			forward.y = forward.y - 1;

		if (in->getKeyState(340) || in->getKeyState(16))
			speed *= 5;

		camera.rotation = camera.rotation + (rotate * Time::deltaTime * 1000);
		camera.position = camera.position + forward.normalized() * speed * Time::deltaTime;

		for (int i = 0; i < m.size(); i++)
			m[i].rotation = m[i].rotation + Vec3{ 1, 1, 1 } *Time::deltaTime;

		cameraCorrection();

		drwn->beginRender();
		drwn->clear();
		dirLight.sendToShader(shader);
		Mesh::renderAll(shader, camera, &projectionMatrix, m);
		drwn->endRender();

		float xoffset = mouseX - lastX;
		float yoffset = lastY - mouseY;
		lastX = mouseX;
		lastY = mouseY;

		xoffset *= sensitivity;
		yoffset *= sensitivity;
		engine3D->camera.rotation.y += xoffset * Time::deltaTime;
		engine3D->camera.rotation.x -= yoffset * Time::deltaTime;
		((Game*)engine3D)->cameraCorrection();
	}

	static inline float mouseX = 0, mouseY = 0;
	static inline float lastX = 800, lastY = 450;
	static inline const float sensitivity = 0.2f;
	void onClose() {
		memory::safe_delete(shader);
	}

	void cameraCorrection() {
		if (abs(camera.rotation.x) > 1.527163)
			camera.rotation.x = 1.527163 * utils3d::sign(camera.rotation.x);
		if (abs(camera.rotation.y) > 6.28319)
			camera.rotation.y = 0;
	}

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		mouseX = xpos;
		mouseY = ypos;
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
