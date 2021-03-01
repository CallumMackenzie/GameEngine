#include "Engine.h"

// Callum Mackenzie

Engine* Engine::engine;

Engine* Engine::getEngine()
{
	if (engine == nullptr) {
		engine = new Engine();
	}
	return engine;
}

Engine::Engine() 
{
	running = true;
}

Engine::~Engine() {
	stop();
	memory::safe_delete<Direct2DWindow>(drwn);
	memory::safe_delete<WindowClass>(primeClass);
}

void Engine::stop() 
{
	onClose();
	running = false;
	memory::safe_delete(Input::input);
	memory::safe_delete(Physics2D::physics2D);
}

Sprite* control1;
Sprite* box1;
Sprite* boxControl1;

Sprite* circles[40];
int numCircles = 40;

void Engine::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	// HICON hIcon = LoadIconA(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	primeClass = new WindowClass(L"Ingenium WC", hInstance);
	primeClass->setWindowProc(DEFAULT_WND_PROC);
	primeClass->wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//primeClass->wc.hIcon = hIcon;
	//primeClass->wc.hIconSm = hIcon;
	primeClass->registerClass();

	RootWindow* win = new RootWindow(hInstance, primeClass, L"Ingenium", CW_USEDEFAULT, CW_USEDEFAULT, 900, 1600);
	win->style = WS_SYSMENU;
	win->create();
	// win->setFullscreen();
	win->show();
	
	////HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE());

	drwn = new Direct2DWindow(win);

	ID2D1Bitmap* bmp;
	ID2D1Bitmap* munkey;
	HRESULT success = drwn->loadFileBitmap(L"C:\\Users\\Alexx\\source\\repos\\GameEngine\\Debug\\EST.png", 15, 240, &bmp);
	success = drwn->loadFileBitmap(L"C:\\Users\\Alexx\\source\\repos\\GameEngine\\Debug\\Munkey.png", 500, 500, &munkey);

	if (SUCCEEDED(success)) {
		control1 = new Sprite("ElectrostaticTube", Vector2(-50, -50), Rotation(10, 10, 10), bmp, Hitbox2D::createRectHitbox(Vector2(), Vector2(30, 60)));
		control1->rotation.centre[0] = 15;
		control1->rotation.centre[1] = 30;
		control1->size = Vector2(30, 60);
		control1->transparency = 0.6f;
		control1->frameData.frameHeight = 30;
		control1->frameData.frameWidth = 15;
		control1->frameData.frames = 8;
		control1->scale = Vector2(1, 1);
		control1->frameData.spriteSheetDirection = Renderable<void>::FrameData::SPRITESHEET_VERTICAL;
		control1->frameData.frameTime = 0.1;
		
		box1 = new Sprite("MunkeyBox", Vector2(-200, -300), Rotation(), munkey, Hitbox2D::createRectHitbox(Vector2(), Vector2(100, 100)));
		box1->size = Vector2(100, 100);
		box1->transparency = 0.4;

		boxControl1 = new Sprite("MunkeyCircle", Vector2(30, 30), Rotation(), munkey, Hitbox2D::createCircleHitbox(70, Vector2(50, 50)));
		boxControl1->size = Vector2(100, 100);

		for (int i = 0; i < (numCircles / 2); i++) {
			circles[i] = new Sprite("MunkeyCircle" + i, Vector2(1 + (80 * i), 200), Rotation(), munkey, Hitbox2D::createCircleHitbox(15 * ((i % 3) + 1), Vector2(30, 30)));
			circles[i]->size = Vector2(1, 1);
		}
		for (int j = (numCircles / 2); j < numCircles; j++) {
			int i = j - (numCircles / 2);
			circles[j] = new Sprite("MunkeyCircle" + j, Vector2(1 + (80 * i), 500), Rotation(), munkey, Hitbox2D::createCircleHitbox(15 * ((i % 3) + 1), Vector2(30, 30)));
			circles[j]->size = Vector2(1, 1);
		}
	}

	drwn->clearColour = D2D1::ColorF::White;
}

void Engine::onUpdate() {
	control1->frameData.calculateFrame();
	Vector2 mousePos = Input::getInput()->getHWNDCursorPos(drwn->getWindow()->getHWND());
	Vector2 add = Vector2(
		mousePos.x() - (boxControl1->position.x() + boxControl1->size.x() / 2),
		mousePos.y() - (boxControl1->position.y() + boxControl1->size.y() / 2));
	float mvspd = 0.05;
	if (Input::getInput()->getKeyState(68) || Input::getInput()->getKeyState(65) || Input::getInput()->getKeyState(87) || Input::getInput()->getKeyState(83)) {
		control1->velocity.add(((Input::getInput()->getKeyState(68) ? mvspd : 0) + (Input::getInput()->getKeyState(65) ? -mvspd : 0)) * Time::getTime()->deltaTime,
			((Input::getInput()->getKeyState(83) ? mvspd : 0) + (Input::getInput()->getKeyState(87) ? -mvspd : 0)) * Time::getTime()->deltaTime);
	}
	if (Input::getInput()->getMouseButton(0) && Vector::qSqrt(add.x() * add.x() + add.y() * add.y()) >= 30) {
		add.normalize();
		boxControl1->velocity.add(add);
	}
	if (Input::getInput()->getKeyState(49)) {
		control1->setXY(0, 0);
		boxControl1->setXY(400, 10);
	}

	boxControl1->addXY(boxControl1->velocity);

	control1->velocity.multiply(0.9, 0.9);
	boxControl1->velocity.multiply(0.92, 0.92);
	control1->rotation.z += 0.1 * Time::getTime()->deltaTime;

	control1->addXY(control1->velocity);

	Physics2D::CollisionData collisionResult = Physics2D::getPhysics2D()->colliding(box1->hitbox2D, control1->hitbox2D, Vector2(), control1->velocity);
	if (collisionResult.direction != Physics2D::COLLISION_NONE) {
		if (Physics2D::isCollisionDown(collisionResult.direction)) {
			control1->setY(box1->position.y() - control1->size.y());
		}
		else if (Physics2D::isCollisionUp(collisionResult.direction)) {
			control1->setY(box1->position.y() + box1->size.y());
		}
		if (Physics2D::isCollisionRight(collisionResult.direction)) {
			control1->setX(box1->position.x() + box1->size.x());
		}
		else if (Physics2D::isCollisionLeft(collisionResult.direction)) {
			control1->setX(box1->position.x() - control1->size.x());
		}
	}
	collisionResult = Physics2D::getPhysics2D()->colliding(control1->hitbox2D, boxControl1->hitbox2D);
	if (collisionResult.direction != Physics2D::COLLISION_NONE) {
		boxControl1->subtractXY(collisionResult.hitVector);
	}
	for (int i = 0; i < numCircles; i++) {
		if (circles[i]->position.x() + circles[i]->hitbox2D.circleRadius() >= 1600) {
			circles[i]->setX(1599 - circles[i]->hitbox2D.circleRadius());
			circles[i]->velocity.x(-circles[i]->velocity.x() * 0.6);
		}
		if (circles[i]->position.x() - circles[i]->hitbox2D.circleRadius() <= 0) {
			circles[i]->setX(circles[i]->hitbox2D.circleRadius());
			circles[i]->velocity.x(-circles[i]->velocity.x() * 0.6);
		}
		if (circles[i]->position.y() + circles[i]->hitbox2D.circleRadius() >= 800) {
			circles[i]->setY(799 - circles[i]->hitbox2D.circleRadius());
			circles[i]->velocity.y(-circles[i]->velocity.y() * 0.6);
		}
		if (circles[i]->position.y() - circles[i]->hitbox2D.circleRadius() <= 0) {
			circles[i]->setY(circles[i]->hitbox2D.circleRadius());
			circles[i]->velocity.y(-circles[i]->velocity.y() * 0.6);
		}
		circles[i]->addXY(circles[i]->velocity);
		//collisionResult = Physics2D::getPhysics2D()->colliding(control1->hitbox2D, circles[i]->hitbox2D);
		//if (collisionResult.direction != Physics2D::COLLISION_NONE) {
		//	collisionResult.hitVector.divide(2, 2);
		//	circles[i]->subtractXY(collisionResult.hitVector);
		//	control1->addXY(collisionResult.hitVector);
		//}
		collisionResult = Physics2D::getPhysics2D()->colliding(circles[i]->hitbox2D, boxControl1->hitbox2D);
		if (collisionResult.direction != Physics2D::COLLISION_NONE) {
			collisionResult.hitVector.divide(2, 2);
			boxControl1->subtractXY(collisionResult.hitVector);
			circles[i]->velocity.add(collisionResult.hitVector);
		}
		for (int j = 0; j < numCircles; j++) {
			if (circles[j]->name != circles[i]->name) {
				collisionResult = Physics2D::getPhysics2D()->colliding(circles[i]->hitbox2D, circles[j]->hitbox2D);
				if (collisionResult.direction != Physics2D::COLLISION_NONE) {
					collisionResult.hitVector.divide(2, 2);
					circles[j]->velocity.subtract(collisionResult.hitVector);
					circles[i]->velocity.add(collisionResult.hitVector);
				}
			}
		}
		drwn->addToRenderQueue(circles[i], Direct2DWindow::RenderLinkedList::TYPE_RENDER_SPRITE);
	}

	drwn->addToRenderQueue(boxControl1, Direct2DWindow::RenderLinkedList::TYPE_RENDER_SPRITE);
	drwn->addToRenderQueue(control1, Direct2DWindow::RenderLinkedList::TYPE_RENDER_SPRITE);
	drwn->addToRenderQueue(box1, Direct2DWindow::RenderLinkedList::TYPE_RENDER_SPRITE);

	drwn->beginRender();
	drwn->drawQueue(false);
	drwn->endRender();
}

void Engine::onClose() {
	memory::safe_delete(box1);
	memory::safe_delete(control1);
	memory::safe_delete(boxControl1);
}

LRESULT CALLBACK Engine::DEFAULT_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		Engine::getEngine()->stop();
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		Input::getInput()->MouseClick[0] = true;
		break;
	case WM_LBUTTONUP:
		Input::getInput()->MouseClick[0] = false;
		break;
	case WM_RBUTTONDOWN:
		Input::getInput()->MouseClick[1] = true;
		break;
	case WM_RBUTTONUP:
		Input::getInput()->MouseClick[1] = false;
		break;
	case WM_MBUTTONDOWN:
		Input::getInput()->MouseClick[2] = true;
		break;
	case WM_MBUTTONUP:
		Input::getInput()->MouseClick[2] = false;
		break;
	case WM_MOUSELEAVE || WM_NCMOUSELEAVE || WM_SETFOCUS:
		Input::getInput()->MouseClick[0] = false;
		Input::getInput()->MouseClick[1] = false;
		Input::getInput()->MouseClick[2] = false;
		break;
	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
};