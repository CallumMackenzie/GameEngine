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
	if (running) {
		stop();
	}
	memory::safe_delete<Direct2DWindow>(drwn);
	memory::safe_delete<WindowClass>(primeClass);
}
void Engine::stop()
{
	running = false;
	onClose();
	memory::safe_delete(Input::input);
	memory::safe_delete(Physics2D::physics2D);
}

void Engine::init(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	primeClass = new WindowClass(L"Ingenium WC", hInstance);
	primeClass->setWindowProc(DEFAULT_WND_PROC);
	primeClass->wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	primeClass->registerClass();

	RootWindow* win = new RootWindow(hInstance, primeClass, L"Ingenium", CW_USEDEFAULT, CW_USEDEFAULT, 900, 1600);
	win->style = WS_SYSMENU | WS_SIZEBOX;
	win->create();
	win->show();

	drwn = new Direct2DWindow(win);

	drwn->drawQueue(false);

#if defined(SCRIPT_LUA)
	loadToLua();
	ingenium_lua::loadFileA(windows::fileAbsolutePathA(LUA_ENGINE_ENTRY));
	ingenium_lua::executeChunk();
	ingenium_lua::executeFunc(LUA_ENGINE_INIT);
#endif
}

void Engine::onUpdate() {
#if defined(SCRIPT_LUA)
	ingenium_lua::executeFunc(LUA_ENGINE_UPDATE);
#endif
}

void Engine::onClose() {
#if defined(SCRIPT_LUA)
	ingenium_lua::executeFunc(LUA_ENGINE_CLOSE);
	ingenium_lua::stopLua();
#endif
}

LRESULT CALLBACK Engine::DEFAULT_WND_PROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		if (Engine::getEngine()->drwn) {
			Engine::getEngine()->drwn->resizePRT(LOWORD(lParam), HIWORD(lParam));
			Engine::getEngine()->drwn->calculateRPR();
		}
		break;
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

#if defined(SCRIPT_LUA)
namespace lua_funcs
{
	template <typename T> T* uDataToPtr(void* data) {
		return *(T**)data;
	}

	template <typename T> T* getSelfAsUData(lua_State* L, int index, const char* inheritName) {
		using namespace ingenium_lua;
		void* ud = 0;
		luaL_checktype(L, index, LUA_TTABLE);
		lua_getfield(L, index, "__self");
		ud = lua_touserdata(L, index + 1);
		T* ret = uDataToPtr<T>(ud);
		// luaL_argcheck(L, ud != 0, "Inheritable class type expected but not found.");
		return ret;
	}

	namespace d2d
	{
		ingenium_lua::LuaClass<Direct2DWindow> iClass = ingenium_lua::LuaClass<Direct2DWindow>("D2D");

		int setDRWNSize(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (number, number).", nargs);
			UINT width = luaL_checkinteger(lua, 1);
			UINT height = luaL_checkinteger(lua, 2);

			lua_pop(lua, 2);

			Engine::getEngine()->drwn->setSize(width, height);
			return 0;
		}
		int getMouseXDRWN(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 0)
				return luaL_error(lua, "Got %d arguments, expected 0.", nargs);
			lua_pushnumber(lua, Engine::getEngine()->drwn->getMouseX());
			return 1;
		}
		int getMouseYDRWN(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 0)
				return luaL_error(lua, "Got %d arguments, expected 0.", nargs);
			lua_pushnumber(lua, Engine::getEngine()->drwn->getMouseY());
			return 1;
		}
		int printDRWN(lua_State* lua) {
			int nargs = lua_gettop(lua);
			for (int i = 0; i < nargs; i++) {
				Debug::oss << luaL_checkstring(lua, i + 1);
				Debug::write();
			}
			lua_pop(lua, nargs);
			return 0;
		}
		int renderDRWN(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (boolean).", nargs);
			bool clear = lua_toboolean(lua, 1);
			Engine::getEngine()->drwn->beginRender();
			Engine::getEngine()->drwn->drawQueue(clear);
			Engine::getEngine()->drwn->endRender();
			lua_pop(lua, 1);
			return 0;
		}
		int setDRWNFullScreen(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 0)
				return luaL_error(lua, "Got %d arguments, expected 0.", nargs);
			Engine::getEngine()->drwn->window->setFullscreen();
		}
		int setDRWNClearColour(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (int).", nargs);
			UINT32 colour = luaL_checkinteger(lua, 1);
			Engine::getEngine()->drwn->clearColour = D2D1::ColorF(colour);
		}
		int isKeyPressed(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (integer).", nargs);
			int key = luaL_checkinteger(lua, 1);
			lua_pop(lua, 1);
			bool keyPressed = Input::getInput()->getKeyState(key);
			lua_pushboolean(lua, keyPressed);
			return 1;
		}
		void registerDRWN(lua_State* lua)
		{
			using namespace ingenium_lua;

			iClass.addMetaMethod(lua, lua_func("setSize", setDRWNSize));
			iClass.addMetaMethod(lua, lua_func("render", renderDRWN));
			iClass.addMetaMethod(lua, lua_func("getMouseX", getMouseXDRWN));
			iClass.addMetaMethod(lua, lua_func("getMouseY", getMouseYDRWN));
			iClass.addMetaMethod(lua, lua_func("write", printDRWN));
			iClass.addMetaMethod(lua, lua_func("setFullscreen", setDRWNFullScreen));
			iClass.addMetaMethod(lua, lua_func("setClearColour", setDRWNClearColour));
			iClass.addMetaMethod(lua, lua_func("keyPressed", isKeyPressed));

			iClass.registerClass(lua);
		}
	}
	namespace time 
	{
		ingenium_lua::LuaClass<Direct2DWindow> iClass = ingenium_lua::LuaClass<Direct2DWindow>("Time");

		int getDeltaTime(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 0)
				return luaL_error(lua, "Got %d arguments, expected 0.", nargs);

			lua_pushnumber(lua, Time::getTime()->deltaTime);
			return 1;
		}
		int setFPS(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (number).", nargs);

			float newFPS = luaL_checknumber(lua, 1);
			lua_pop(lua, 1);
			Time::getTime()->setFPS(newFPS);

			return 0;
		}

		void registerTime(lua_State* lua) {
			using namespace ingenium_lua;

			iClass.addMetaMethod(lua, lua_func("deltaTime", getDeltaTime));
			iClass.addMetaMethod(lua, lua_func("setFPS", setFPS));

			iClass.registerClass(lua);
		}
	}
	namespace vec2
	{
		ingenium_lua::LuaClass<Vector2> iClass = ingenium_lua::LuaClass<Vector2>("Vector2");

		constexpr int exargs = 1;

		int newVector2(lua_State* lua);

		int magnitude(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pushnumber(lua, Vector2::hypotenuse(*v2));
			return 1;
		}
		int normalize(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			v2->normalize();
			return 0;
		}
		int getX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);

			Vector2* v2 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pushnumber(lua, v2->x());
			return 1;
		}
		int getY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pushnumber(lua, v2->y());
			return 1;
		}
		int setX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);
			float nv = luaL_checknumber(lua, 2);
			lua_pop(lua, 1);
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 1);
			v2->x(nv);
			return 0;
		}
		int setY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);
			float nv = luaL_checknumber(lua, 2);
			lua_pop(lua, 1);
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 1);
			v2->y(nv);
			return 0;
		}
		int add(lua_State* lua) {
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 2, iClass.metaName);
			lua_pop(lua, 2);
			Vector2* v1 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);
			lua_getglobal(lua, iClass.name);
			lua_pushnumber(lua, v1->xVal + v2->xVal);
			lua_pushnumber(lua, v1->yVal + v2->yVal);
			return newVector2(lua);
		}
		int subtract(lua_State* lua) {
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 2, iClass.metaName);
			lua_pop(lua, 2);
			Vector2* v1 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);
			lua_getglobal(lua, iClass.name);
			lua_pushnumber(lua, v1->xVal - v2->xVal);
			lua_pushnumber(lua, v1->yVal - v2->yVal);
			return newVector2(lua);
		}
		int multiply(lua_State* lua) {
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 2, iClass.metaName);
			lua_pop(lua, 2);
			Vector2* v1 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);
			lua_getglobal(lua, iClass.name);
			lua_pushnumber(lua, v1->xVal * v2->xVal);
			lua_pushnumber(lua, v1->yVal * v2->yVal);
			return newVector2(lua);
		}
		int divide(lua_State* lua) {
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 2, iClass.metaName);
			lua_pop(lua, 2);
			Vector2* v1 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);
			lua_getglobal(lua, iClass.name);
			lua_pushnumber(lua, v1->xVal / v2->xVal);
			lua_pushnumber(lua, v1->yVal / v2->yVal);
			return newVector2(lua);
		}
		int unaryMinus(lua_State* lua) {
			Vector2* v1 = getSelfAsUData<Vector2>(lua, 2, iClass.metaName);
			lua_pop(lua, 3);
			lua_getglobal(lua, iClass.name);
			lua_pushnumber(lua, -v1->xVal);
			lua_pushnumber(lua, -v1->yVal);
			return newVector2(lua);
		}
		int floorDiv(lua_State* lua) {
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 2, iClass.metaName);
			lua_pop(lua, 2);
			Vector2* v1 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);
			lua_getglobal(lua, iClass.name);
			lua_pushnumber(lua, (int)(v1->xVal / v2->xVal));
			lua_pushnumber(lua, (int)(v1->yVal / v2->yVal));
			return newVector2(lua);
		}
		int toString(lua_State* lua) {
			Vector2* v2 = getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			std::string v2s("vec2(");
			v2s = v2s.append(std::to_string(v2->x())).append(", ").append(std::to_string(v2->y())).append(")");
			lua_pushstring(lua, v2s.c_str());
			return 1;
		}
		int getNormalized(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Vector2 v2 = *getSelfAsUData<Vector2>(lua, 1, iClass.metaName);
			lua_pop(lua, 1);
			
			v2.normalize();
			lua_pushnumber(lua, v2.x());
			lua_pushnumber(lua, v2.y());
			return newVector2(lua);
		}

		int newVector2(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 0 + exargs && nargs != 2 + exargs)
				return luaL_error(lua, "Got %d arguments, expected 0 or 2: (number, number).", nargs);

			float xComp = 0;
			float yComp = 0;
			if (nargs == 2 + exargs) {
				xComp = luaL_checknumber(lua, 1 + exargs);
				yComp = luaL_checknumber(lua, 2 + exargs);
				lua_pop(lua, 2);
			}
			Vector2* vec2 = new Vector2(xComp, yComp);

			iClass.createInstance(lua, vec2);
			return 1;
		}

		int free(lua_State* lua) {
			return ingenium_lua::free<Vector2>(lua);
		}

		void registerVector2(lua_State* lua) {
			using namespace ingenium_lua;
			iClass.addMethod(lua, lua_func("new", newVector2));
			iClass.addMetaMethod(lua, lua_func("new", newVector2));

			iClass.addMetaMethod(lua, lua_func("__gc", free));
			iClass.addMetaMethod(lua, lua_func("__tostring", toString));
			iClass.addMetaMethod(lua, lua_func("__add", add));
			iClass.addMetaMethod(lua, lua_func("__sub", subtract));
			iClass.addMetaMethod(lua, lua_func("__mul", multiply));
			iClass.addMetaMethod(lua, lua_func("__div", divide));
			iClass.addMetaMethod(lua, lua_func("__unm", unaryMinus));
			iClass.addMetaMethod(lua, lua_func("__idiv", floorDiv));

			iClass.addMethod(lua, lua_func("getX", getX));
			iClass.addMethod(lua, lua_func("getY", getY));
			iClass.addMethod(lua, lua_func("setX", setX));
			iClass.addMethod(lua, lua_func("setY", setY));
			iClass.addMethod(lua, lua_func("normalize", normalize));
			iClass.addMethod(lua, lua_func("normalized", getNormalized));
			iClass.addMethod(lua, lua_func("magnitude", magnitude));

			iClass.registerClass(lua);
		}
#ifdef CONSTRUCTOR_METHOD_NAME
#undef CONSTRUCTOR_METHOD_NAME
#endif
	}
	namespace hitbox2D
	{
		ingenium_lua::LuaClass<Hitbox2D> iClass = ingenium_lua::LuaClass<Hitbox2D>("Hitbox2D");

		int free(lua_State* lua) {
			return ingenium_lua::free<Hitbox2D>(lua);
		}

		int toString(lua_State* lua) {
			Hitbox2D* v2 = getSelfAsUData<Hitbox2D>(lua, 1, iClass.metaName);
			std::string v2s("Hitbox2D(");
			switch (v2->type) {
			case Hitbox2D::TYPE_RECTANGLE:
				v2s = v2s.append("TYPE_RECTANGLE (").append(std::to_string(Hitbox2D::TYPE_RECTANGLE)).append("), pos=(").append(std::to_string(v2->rectPos().x())).append(", ").append(std::to_string(v2->rectPos().y())).
					append("), width=").append(std::to_string(v2->rectSize().x())).append(", height=").append(std::to_string(v2->rectSize().y()));
				break;
			case Hitbox2D::TYPE_CIRCLE:
				v2s = v2s.append("TYPE_CIRCLE (").append(std::to_string(Hitbox2D::TYPE_CIRCLE)).append("), r=").append(std::to_string(v2->circleRadius()).append(", ").
					append("centre=(").append(std::to_string(v2->circleCentre().x()))).append(", ").append(std::to_string(v2->circleCentre().y())).append(")");
				break;
			default:
				v2s = v2s.append("TYPE_UNKNOWN (").append(std::to_string(Hitbox2D::TYPE_UNDEFINED)).append(")");
				break;
			}
			v2s = v2s.append(")");
			lua_pushstring(lua, v2s.c_str());
			return 1;
		}
		int getType(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Hitbox2D* v2 = getSelfAsUData<Hitbox2D>(lua, 1, iClass.metaName);
			lua_pushnumber(lua, v2->type);
			return 1;
		}
		int sizeX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);

			Hitbox2D* v2 = getSelfAsUData<Hitbox2D>(lua, 1, iClass.metaName);
			switch (v2->type) {
			case Hitbox2D::TYPE_RECTANGLE:
				lua_pushnumber(lua, v2->rectSize().x());
				break;
			case Hitbox2D::TYPE_CIRCLE:
				lua_pushnumber(lua, v2->circleRadius());
				break;
			default:
				return luaL_error(lua, "No size for HITBOX2D_TYPE_UNDEFINED.");
			}
			return 1;
		}
		int sizeY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Hitbox2D* v2 = getSelfAsUData<Hitbox2D>(lua, 1, iClass.metaName);
			switch (v2->type) {
			case Hitbox2D::TYPE_RECTANGLE:
				lua_pushnumber(lua, v2->rectSize().y());
				break;
			case Hitbox2D::TYPE_CIRCLE:
				lua_pushnumber(lua, v2->circleRadius());
				break;
			default:
				return luaL_error(lua, "No size for HITBOX2D_TYPE_UNDEFINED.");
			}
			return 1;
		}
		int xPosition(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Hitbox2D* v2 = getSelfAsUData<Hitbox2D>(lua, 1, iClass.metaName);
			switch (v2->type) {
			case Hitbox2D::TYPE_RECTANGLE:
				lua_pushnumber(lua, v2->rectPos().x());
				break;
			case Hitbox2D::TYPE_CIRCLE:
				lua_pushnumber(lua, v2->circleCentre().x());
				break;
			default:
				return luaL_error(lua, "No position for HITBOX2D_TYPE_UNDEFINED.");
			}
			return 1;
		}
		int yPosition(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Hitbox2D* v2 = getSelfAsUData<Hitbox2D>(lua, 1, iClass.metaName);
			switch (v2->type) {
			case Hitbox2D::TYPE_RECTANGLE:
				lua_pushnumber(lua, v2->rectPos().y());
				break;
			case Hitbox2D::TYPE_CIRCLE:
				lua_pushnumber(lua, v2->circleCentre().y());
				break;
			default:
				return luaL_error(lua, "No position for HITBOX2D_TYPE_UNDEFINED.");
			}
			return 1;
		}
		int radius(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);

			Hitbox2D* v2 = getSelfAsUData<Hitbox2D>(lua, 1, iClass.metaName);

			switch (v2->type) {
			case Hitbox2D::TYPE_CIRCLE:
				lua_pushnumber(lua, v2->circleRadius());
				break;
			case Hitbox2D::TYPE_RECTANGLE:
				return luaL_error(lua, "No radius for HITBOX2D_TYPE_RECTANGLE.");
			default:
				return luaL_error(lua, "No radius for HITBOX2D_TYPE_UNDEFINED.");
			}

			return 1;
		}

		int newHitbox2D(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1 && nargs != 3)
				return luaL_error(lua, "Got %d arguments, expected 0 or 2: (Vector2, Vector2) or (number, Vector2).", nargs);

			Hitbox2D* hit = nullptr;

			if (nargs == 1) {
				hit = Hitbox2D::createUndefinedHitboxPtr();
			}
			else {
				switch (lua_type(lua, 2)) {
				case LUA_TNUMBER:
				{
					Vector2* v2 = getSelfAsUData<Vector2>(lua, 3, vec2::iClass.metaName);
					lua_pop(lua, 2);
					float diameter = luaL_checknumber(lua, 2);
					lua_pop(lua, 1);
					hit = Hitbox2D::createCircleHitboxPtr(diameter, *v2);
				}
				break;
				default:
					Vector2* v2 = getSelfAsUData<Vector2>(lua, 3, iClass.metaName);
					lua_pop(lua, 2);
					Vector2* v1 = getSelfAsUData<Vector2>(lua, 2, iClass.metaName);
					lua_pop(lua, 2);
					hit = Hitbox2D::createRectHitboxPtr(*v1, *v2);
					break;
				}
			}

			iClass.createInstance(lua, hit);

			return 1;
		}

		void registerHitbox2D(lua_State* lua) {
			using namespace ingenium_lua;
			iClass.addMetaMethod(lua, lua_func("new", newHitbox2D));
			iClass.addMetaMethod(lua, lua_func("newUndefined", newHitbox2D));
			iClass.addMetaMethod(lua, lua_func("newCircle", newHitbox2D));
			iClass.addMetaMethod(lua, lua_func("newRect", newHitbox2D));
			iClass.addMetaMethod(lua, lua_func("__gc", free));
			iClass.addMetaMethod(lua, lua_func("__tostring", toString));

			iClass.addMethod(lua, lua_func("type", getType));
			iClass.addMethod(lua, lua_func("yPos", yPosition));
			iClass.addMethod(lua, lua_func("xPos", xPosition));
			iClass.addMethod(lua, lua_func("radius", radius));
			iClass.addMethod(lua, lua_func("width", sizeX));
			iClass.addMethod(lua, lua_func("height", sizeY));

			iClass.registerClass(lua);

			lua_pushnumber(lua, Hitbox2D::TYPE_UNDEFINED);
			lua_setglobal(lua, "HITBOX2D_TYPE_UNDEFINED");
			lua_pushnumber(lua, Hitbox2D::TYPE_CIRCLE);
			lua_setglobal(lua, "HITBOX2D_TYPE_CIRCLE");
			lua_pushnumber(lua, Hitbox2D::TYPE_RECTANGLE);
			lua_setglobal(lua, "HITBOX2D_TYPE_RECTANGLE");
		}
#ifdef CONSTRUCTOR_METHOD_NAME
#undef CONSTRUCTOR_METHOD_NAME
#endif
	}
	namespace sprite
	{
		ingenium_lua::LuaClass<Sprite> iClass = ingenium_lua::LuaClass<Sprite>("Sprite");

		int free(lua_State* lua) {
			return ingenium_lua::free<Sprite>(lua);
		}

		int toString(lua_State* lua) {
			ingenium_lua::printStackTrace();
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);
			std::string str ("sprite(");
			str = str.append(sp->name).append(")");
			lua_pushstring(lua, str.c_str());
			return 1;
		}
		int render(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			Engine::getEngine()->drwn->addToRenderQueue(sp, Direct2DWindow::RenderLinkedList::TYPE_RENDER_SPRITE);
			return 0;
		}
		int setHitbox2D(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, Hitbox2D).", nargs);

			Hitbox2D* h2d = getSelfAsUData<Hitbox2D>(lua, 2, hitbox2D::iClass.metaName);
			lua_pop(lua, 2);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			sp->hitbox2D = *h2d;
			return 0;
		}
		int setX (lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);

			float xV = lua_tonumber(lua, 2);
			lua_pop(lua, 1);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			sp->setX(xV);
			return 0;
		}
		int setY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);

			float yV = lua_tonumber(lua, 2);
			lua_pop(lua, 1);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			sp->setY(yV);
			return 0;
		}
		int setXY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 3)
				return luaL_error(lua, "Got %d arguments, expected 3: (self, number, number).", nargs);

			float yV = lua_tonumber(lua, 3);
			lua_pop(lua, 1);
			float xV = lua_tonumber(lua, 2);
			lua_pop(lua, 1);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			sp->setXY(xV, yV);
			return 0;
		}
		int getX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);

			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			lua_pushnumber(lua, sp->position.x());
			return 1;
		}
		int getY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);

			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			lua_pushnumber(lua, sp->position.y());
			return 1;
		}
		int addX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);

			float xV = lua_tonumber(lua, 2);
			lua_pop(lua, 1);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			sp->addXY(xV, 0);
			return 0;
		}
		int addY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);

			float yV = lua_tonumber(lua, 2);
			lua_pop(lua, 1);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			sp->addXY(0, yV);
			return 0;
		}
		int addXY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 3)
				return luaL_error(lua, "Got %d arguments, expected 3: (self, number, number).", nargs);

			float yV = lua_tonumber(lua, 3);
			lua_pop(lua, 1);
			float xV = lua_tonumber(lua, 2);
			lua_pop(lua, 1);
			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			sp->addXY(xV, yV);
			return 0;
		}
		int getHitbox(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);

			ingenium_lua::printStackTrace();

			Sprite* sp = getSelfAsUData<Sprite>(lua, 1, iClass.metaName);
			lua_pop(lua, 2);

			Hitbox2D* hb2d = Hitbox2D::createUndefinedHitboxPtr();
			*hb2d = sp->hitbox2D;

			lua_getglobal(lua, hitbox2D::iClass.name);
			hitbox2D::iClass.createInstance(lua, hb2d);
			lua_remove(lua, -2);
			return 1;
		}

		int newSprite(lua_State* lua) {
			// name, path --- 3
			// name, path, frameSize, frames, frameTime, spriteSheetDirection --- 7
			// name, path, frameSize, frames, frameTime, spriteSheetDirection, callback_function, callback_function_frame --- 8

			int nargs = lua_gettop(lua);
			if (nargs != 3 && nargs != 8 && nargs != 10)
				return luaL_error(lua, "Got %d arguments, expected 2, 7, or 9.", nargs);

			Sprite* sp = nullptr;

			switch (nargs) {
			case 3:
			{
				Sprite::FrameData fd = Sprite::FrameData();
				fd.frameWidth = 500;
				fd.frameHeight = 500;
				const char* name = lua_tostring(lua, 2);
				std::wstring path = string_conversion::widen(lua_tostring(lua, 3));

				sp = Sprite::createSpriteFromName(name, path.c_str(), fd, Engine::getEngine()->drwn->pRT);
				sp->size.x(100);
				sp->size.y(100);
				lua_pop(lua, 2);
				break;
			}
			case 7:
			{
				Hitbox2D* hb2d = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 3));
				Sprite::FrameData fd = Sprite::FrameData();
				sp = Sprite::createSpriteFromName(lua_tostring(lua, 2), (LPCWSTR)lua_tostring(lua, 3), fd, Engine::getEngine()->drwn->pRT);
				lua_pop(lua, 6);
				break;
			}
			default:
				break;
			}

			iClass.createInstance(lua, sp);

			return 1;
		}

		void registerSprite(lua_State* lua) {
			using namespace ingenium_lua;

			iClass.addMetaMethod(lua, lua_func("new", newSprite));
			iClass.addMetaMethod(lua, lua_func("__gc", free));
			iClass.addMetaMethod(lua, lua_func("__tostring", toString));

			iClass.addMethod(lua, lua_func("addToRender", render));
			iClass.addMethod(lua, lua_func("setHitbox2D", setHitbox2D));
			iClass.addMethod(lua, lua_func("setX", setX));
			iClass.addMethod(lua, lua_func("setY", setY));
			iClass.addMethod(lua, lua_func("setXY", setXY));
			iClass.addMethod(lua, lua_func("getX", getX));
			iClass.addMethod(lua, lua_func("getY", getY));

			iClass.addMethod(lua, lua_func("addX", addX));
			iClass.addMethod(lua, lua_func("addY", addY));
			iClass.addMethod(lua, lua_func("addXY", addXY));
			iClass.addMethod(lua, lua_func("getHitbox2D", getHitbox));

			iClass.registerClass(lua);
		}
	}
}

void Engine::loadToLua()
{
	if (!ingenium_lua::state) {
		ingenium_lua::initLua();
	}

	lua_funcs::d2d::registerDRWN(ingenium_lua::state);
	lua_funcs::time::registerTime(ingenium_lua::state);
	lua_funcs::vec2::registerVector2(ingenium_lua::state);
	lua_funcs::hitbox2D::registerHitbox2D(ingenium_lua::state);
	lua_funcs::sprite::registerSprite(ingenium_lua::state);
}
#endif