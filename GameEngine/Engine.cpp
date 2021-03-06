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
const char* Engine::fileAbsolutePath(const char* relativePath)
{
	char str[512];
	GetFullPathNameA(relativePath, 512, str, NULL);
	return str;
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
	ingenium_lua::loadFile(fileAbsolutePath(LUA_ENGINE_ENTRY));
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
	void printStackTrace() {
		Debug::oss << ingenium_lua::getStackTrace();
		Debug::writeLn();
	}
	template <typename T> T* uDataToPtr(void* data) {
		return *(T**)data;
	}

	namespace d2d
	{
#define LUA_D2DWIN_NAME "D2D"
#define LUA_D2DWIN_SETSIZE "setSize"
#define LUA_D2DWIN_GETMOUSEX "getMouseX"
#define LUA_D2DWIN_GETMOUSEY "getMouseY"
#define LUA_D2DWIN_PRINT "write"
#define LUA_D2DWIN_RENDER "render"

		int setDRWNSize(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2.", nargs);
			UINT width = luaL_checkint(lua, 1);
			UINT height = luaL_checkint(lua, 2);

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
				return luaL_error(lua, "Got %d arguments, expected 1.", nargs);
			bool clear = lua_toboolean(lua, 1);
			Engine::getEngine()->drwn->beginRender();
			Engine::getEngine()->drwn->drawQueue(clear);
			Engine::getEngine()->drwn->endRender();
			lua_pop(lua, 1);
			return 0;
		}
		void registerDRWN(lua_State* lua)
		{
			Direct2DWindow** pmPtr = (Direct2DWindow**)lua_newuserdata(lua, sizeof(Direct2DWindow*));
			*pmPtr = Engine::getEngine()->drwn;

			luaL_newmetatable(lua, "D2DMetaTable");

			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");

			luaL_Reg functions[] = {
				 LUA_D2DWIN_SETSIZE, setDRWNSize,
				 LUA_D2DWIN_RENDER, renderDRWN,
				 LUA_D2DWIN_GETMOUSEX, getMouseXDRWN,
				 LUA_D2DWIN_GETMOUSEY, getMouseYDRWN,
				 LUA_D2DWIN_PRINT, printDRWN,
				 nullptr, nullptr
			};

			luaL_register(lua, 0, functions);
			lua_setmetatable(lua, -2);
			lua_setglobal(lua, LUA_D2DWIN_NAME);
		}
	}
	namespace vec2
	{
#define LUA_VECTOR2_NAME "Vector2"
#define LUA_VECTOR2_NAME_INHERITABLE "Ingenium.Vector2"
#define LUA_VECTOR2_GETX "getX"
#define LUA_VECTOR2_SETX "setX"
#define LUA_VECTOR2_GETY "getY"
#define LUA_VECTOR2_SETY "setY"
#define LUA_VECTOR2_NORMALIZE "normalize"
#define LUA_VECTOR2_HYPOTENUSE "hypotenuse"
#define LUA_VECTOR2_MAGNITUDE "magnitude"

		int newVector2(lua_State* lua);

		int magnitude(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			lua_pushnumber(lua, Vector2::hypotenuse(*v2));
			return 1;
		}

		int normalize(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			v2->normalize();
			return 0;
		}

		int getX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);

			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			lua_pushnumber(lua, v2->x());
			return 1;
		}

		int getY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			lua_pushnumber(lua, v2->y());
			return 1;
		}

		int setX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			float nv = luaL_checknumber(lua, 1);
			v2->x(nv);
			return 0;
		}

		int setY(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2: (self, number).", nargs);
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			float nv = luaL_checknumber(lua, 1);
			v2->y(nv);
			return 0;
		}

		int add(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, v1->xVal + v2->xVal);
			lua_pushnumber(lua, v1->yVal + v2->yVal);
			return newVector2(lua);
		}

		int subtract(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, v1->xVal - v2->xVal);
			lua_pushnumber(lua, v1->yVal - v2->yVal);
			return newVector2(lua);
		}

		int multiply(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, v1->xVal * v2->xVal);
			lua_pushnumber(lua, v1->yVal * v2->yVal);
			return newVector2(lua);
		}

		int divide(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, v1->xVal / v2->xVal);
			lua_pushnumber(lua, v1->yVal / v2->yVal);
			return newVector2(lua);
		}

		int unaryMinus(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			lua_pop(lua, 1);
			lua_pushnumber(lua, -v1->xVal);
			lua_pushnumber(lua, -v1->yVal);
			return newVector2(lua);
		}

		int floorDiv(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, (int)(v1->xVal / v2->xVal));
			lua_pushnumber(lua, (int)(v1->yVal / v2->yVal));
			return newVector2(lua);
		}

		int toString(lua_State* lua) {
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			std::string v2s("vec2(");
			v2s = v2s.append(std::to_string(v2->x())).append(", ").append(std::to_string(v2->y())).append(")");
			lua_pushstring(lua, v2s.c_str());
			return 1;
		}

		int FREE(lua_State* lua) {
			memory::safe_delete(*(Vector2**)lua_touserdata(lua, 1));
			return 0;
		}

		int newVector2(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 0 && nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 0 or 2: (number, number).", nargs);

			float xComp = 0;
			float yComp = 0;
			if (nargs == 2) {
				xComp = luaL_checknumber(lua, 1);
				yComp = luaL_checknumber(lua, 2);
				lua_pop(lua, 2);
			}

			Vector2** v2 = (Vector2**)lua_newuserdata(lua, sizeof(Vector2*));
			*v2 = new Vector2(xComp, yComp);

			luaL_newmetatable(lua, LUA_VECTOR2_NAME_INHERITABLE);

			lua_pushstring(lua, "__gc");
			lua_pushcfunction(lua, FREE);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__tostring");
			lua_pushcfunction(lua, toString);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__add");
			lua_pushcfunction(lua, add);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__sub");
			lua_pushcfunction(lua, subtract);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__mul");
			lua_pushcfunction(lua, multiply);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__div");
			lua_pushcfunction(lua, divide);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__unm");
			lua_pushcfunction(lua, unaryMinus);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__idiv");
			lua_pushcfunction(lua, floorDiv);
			lua_settable(lua, -3);

			lua_setmetatable(lua, -2);
			return 1;
		}

		luaL_Reg functions[] = {
			"new", newVector2,
			"toString", toString,
			LUA_VECTOR2_GETX, getX,
			LUA_VECTOR2_GETY, getY,
			LUA_VECTOR2_SETX, setX,
			LUA_VECTOR2_SETY, setY,
			LUA_VECTOR2_NORMALIZE, normalize,
			LUA_VECTOR2_MAGNITUDE, magnitude,
			LUA_VECTOR2_HYPOTENUSE, magnitude,
			nullptr, nullptr
		};

		void registerVector2(lua_State* lua) {
			luaL_newmetatable(lua, LUA_VECTOR2_NAME_INHERITABLE);
			luaL_register(lua, 0, functions);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");
			luaL_register(lua, LUA_VECTOR2_NAME, functions);
		}
	}
	namespace hitbox2D
	{
#define LUA_HITBOX2D_NAME "Hitbox2D"
#define LUA_HITBOX2D_NAME_INHERITABLE "Ingenium.Hitbox2D"
#define LUA_HITBOX2D_GETTYPE "type"
#define LUA_HITBOX2D_GETPOSX "posX"
#define LUA_HITBOX2D_GETPOSY "posY"
#define LUA_HITBOX2D_GETSIZEX "sizeX"
#define LUA_HITBOX2D_GETSIZEY "sizeY"
#define LUA_HITBOX2D_GETRADIUS "radius"

		int toString(lua_State* lua) {
			Hitbox2D* v2 = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 1));
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

		int FREE(lua_State* lua) {
			memory::safe_delete(*(Hitbox2D**)lua_touserdata(lua, 1));
			return 0;
		}

		int getType(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Hitbox2D* v2 = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 1));
			lua_pushnumber(lua, v2->type);
			return 1;
		}

		int sizeX(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Hitbox2D* v2 = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 1));
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
			Hitbox2D* v2 = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 1));
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

		int xPosition (lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 1)
				return luaL_error(lua, "Got %d arguments, expected 1: (self).", nargs);
			Hitbox2D* v2 = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 1));
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
			Hitbox2D* v2 = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 1));
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

			Hitbox2D* v2 = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 1));

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
			if (nargs != 0 && nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 0 or 2: (Vector2, Vector2) or (number, Vector2).", nargs);

			Hitbox2D** hit = (Hitbox2D**)lua_newuserdata(lua, sizeof(Hitbox2D*));
			
			if (nargs == 0) {
				*hit = Hitbox2D::createUndefinedHitboxPtr();
			}
			else {
				switch (lua_type(lua, 1)) {
				case LUA_TNUMBER:
					{
					float diameter = luaL_checknumber(lua, 1);
					Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
					*hit = Hitbox2D::createCircleHitboxPtr(diameter, *v2);
					}
					break;
				default:
					Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
					Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
					*hit = Hitbox2D::createRectHitboxPtr(*v1, *v2);
					break;
				}
				lua_remove(lua, -2);
				lua_remove(lua, -2);
			}

			luaL_newmetatable(lua, LUA_HITBOX2D_NAME_INHERITABLE);
			lua_pushstring(lua, "__gc");
			lua_pushcfunction(lua, FREE);
			lua_settable(lua, -3);
			lua_pushstring(lua, "__tostring");
			lua_pushcfunction(lua, toString);
			lua_settable(lua, -3);

			lua_setmetatable(lua, -2);
			return 1;
		}

		luaL_Reg functions[] = {
			"new", newHitbox2D,
			"newUndefined", newHitbox2D,
			"newCircle", newHitbox2D,
			"newRect", newHitbox2D,
			LUA_HITBOX2D_GETTYPE, getType,
			LUA_HITBOX2D_GETPOSX, xPosition,
			LUA_HITBOX2D_GETPOSY, yPosition,
			LUA_HITBOX2D_GETRADIUS, radius,
			LUA_HITBOX2D_GETSIZEY, sizeY,
			LUA_HITBOX2D_GETSIZEX, sizeX,
			nullptr, nullptr
		};

		void registerHitbox2D(lua_State* lua) {
			luaL_newmetatable(lua, LUA_HITBOX2D_NAME_INHERITABLE);
			luaL_register(lua, 0, functions);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");
			luaL_register(lua, LUA_HITBOX2D_NAME, functions);
				
			lua_pushnumber(lua, Hitbox2D::TYPE_UNDEFINED);
			lua_setglobal(lua, "HITBOX2D_TYPE_UNDEFINED");
			lua_pushnumber(lua, Hitbox2D::TYPE_CIRCLE);
			lua_setglobal(lua, "HITBOX2D_TYPE_CIRCLE");
			lua_pushnumber(lua, Hitbox2D::TYPE_RECTANGLE);
			lua_setglobal(lua, "HITBOX2D_TYPE_RECTANGLE");
		}
	}
}

void Engine::loadToLua()
{
	if (!ingenium_lua::state) {
		ingenium_lua::initLua();
	}

	lua_funcs::d2d::registerDRWN(ingenium_lua::state);
	lua_funcs::vec2::registerVector2(ingenium_lua::state);
	lua_funcs::hitbox2D::registerHitbox2D(ingenium_lua::state);
}
#endif