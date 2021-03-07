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
	void printStackTrace() {
		Debug::oss << ingenium_lua::getStackTrace();
		Debug::writeLn();
	}

	template <typename T> T* uDataToPtr(void* data) {
		return *(T**)data;
	}

	namespace d2d
	{
		constexpr auto CONSTANT_NAME_INHERITABLE = "Ingenium.D2D";
		constexpr auto CONSTANT_NAME = "D2D";
		constexpr auto CONSTANT_SET_SIZE = "setSize";
		constexpr auto CONSTANT_GET_MOUSE_X = "getMouseX";
		constexpr auto CONSTANT_GET_MOUSE_Y = "getMouseY";
		constexpr auto CONSTANT_PRINT = "write";
		constexpr auto CONSTANT_RENDER = "render";

		int setDRWNSize(lua_State* lua) {
			int nargs = lua_gettop(lua);
			if (nargs != 2)
				return luaL_error(lua, "Got %d arguments, expected 2.", nargs);
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

			luaL_Reg functions[] = {
				 CONSTANT_SET_SIZE, setDRWNSize,
				 CONSTANT_RENDER, renderDRWN,
				 CONSTANT_GET_MOUSE_X, getMouseXDRWN,
				 CONSTANT_GET_MOUSE_Y, getMouseYDRWN,
				 CONSTANT_PRINT, printDRWN,
				 nullptr, nullptr
			};

			luaL_newmetatable(lua, CONSTANT_NAME);
			luaL_setfuncs(lua, functions, 0);

			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");

			lua_setmetatable(lua, -2);
			lua_setglobal(lua, CONSTANT_NAME);
		}
	}
	namespace vec2
	{
#define CONSTRUCTOR_METHOD_NAME newVector2
		constexpr auto INSTANCE_NAME_INHERITABLE = "Ingenium.Vector2";
		constexpr auto INSTANCE_NAME = "Vector2";
		constexpr auto INSTANCE_GET_X = "getX";
		constexpr auto INSTANCE_SET_X = "setX";
		constexpr auto INSTANCE_GET_Y = "getY";
		constexpr auto INSTANCE_SET_Y = "setY";
		constexpr auto INSTANCE_NORMALIZE = "normalize";
		constexpr auto INSTANCE_MAGNITUDE = "magnitude";
		constexpr auto INSTANCE_HYPOTENUSE = "hypotenuse";

		int CONSTRUCTOR_METHOD_NAME(lua_State* lua);

		int FREE(lua_State* lua) {
			memory::safe_delete(*(Vector2**)lua_touserdata(lua, 1));
			return 0;
		}

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
			return CONSTRUCTOR_METHOD_NAME(lua);
		}

		int subtract(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, v1->xVal - v2->xVal);
			lua_pushnumber(lua, v1->yVal - v2->yVal);
			return CONSTRUCTOR_METHOD_NAME(lua);
		}

		int multiply(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, v1->xVal * v2->xVal);
			lua_pushnumber(lua, v1->yVal * v2->yVal);
			return CONSTRUCTOR_METHOD_NAME(lua);
		}

		int divide(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, v1->xVal / v2->xVal);
			lua_pushnumber(lua, v1->yVal / v2->yVal);
			return CONSTRUCTOR_METHOD_NAME(lua);
		}

		int unaryMinus(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			lua_pop(lua, 1);
			lua_pushnumber(lua, -v1->xVal);
			lua_pushnumber(lua, -v1->yVal);
			return CONSTRUCTOR_METHOD_NAME(lua);
		}

		int floorDiv(lua_State* lua) {
			Vector2* v1 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 2));
			lua_pop(lua, 2);
			lua_pushnumber(lua, (int)(v1->xVal / v2->xVal));
			lua_pushnumber(lua, (int)(v1->yVal / v2->yVal));
			return CONSTRUCTOR_METHOD_NAME(lua);
		}

		int toString(lua_State* lua) {
			Vector2* v2 = uDataToPtr<Vector2>(lua_touserdata(lua, 1));
			std::string v2s("vec2(");
			v2s = v2s.append(std::to_string(v2->x())).append(", ").append(std::to_string(v2->y())).append(")");
			lua_pushstring(lua, v2s.c_str());
			return 1;
		}

		int CONSTRUCTOR_METHOD_NAME (lua_State* lua) {
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

			luaL_newmetatable(lua, INSTANCE_NAME_INHERITABLE);

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
			"new", CONSTRUCTOR_METHOD_NAME,
			INSTANCE_GET_X, getX,
			INSTANCE_GET_Y, getY,
			INSTANCE_SET_X, setX,
			INSTANCE_SET_Y, setY,
			INSTANCE_NORMALIZE, normalize,
			INSTANCE_MAGNITUDE, magnitude,
			INSTANCE_HYPOTENUSE, magnitude,
			nullptr, nullptr
		};

		void registerVector2(lua_State* lua) {
			luaL_newmetatable(lua, INSTANCE_NAME_INHERITABLE);
			luaL_setfuncs(lua, functions, 0);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");
			lua_setglobal(lua, INSTANCE_NAME);
		}
#ifdef CONSTRUCTOR_METHOD_NAME
#undef CONSTRUCTOR_METHOD_NAME
#endif
	}
	namespace hitbox2D
	{
#define CONSTRUCTOR_METHOD_NAME newHitbox2D
		constexpr auto INSTANCE_NAME_INHERITABLE = "Ingenium.Hitbox2D";
		constexpr auto INSTANCE_NAME = "Hitbox2D";
		constexpr auto INSTANCE_GET_TYPE = "type";
		constexpr auto INSTANCE_GET_POS_X = "posX";
		constexpr auto INSTANCE_GET_POS_Y = "posY";
		constexpr auto INSTANCE_GET_SIZE_X = "sizeX";
		constexpr auto INSTANCE_GET_SIZE_Y = "sizeY";
		constexpr auto INSTANCE_GET_RADIUS = "radius";

		int FREE(lua_State* lua) {
			memory::safe_delete(*(Hitbox2D**)lua_touserdata(lua, 1));
			return 0;
		}

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

		int CONSTRUCTOR_METHOD_NAME (lua_State* lua) {
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

			luaL_newmetatable(lua, INSTANCE_NAME_INHERITABLE);
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
			"new", CONSTRUCTOR_METHOD_NAME,
			"newUndefined", CONSTRUCTOR_METHOD_NAME,
			"newCircle", CONSTRUCTOR_METHOD_NAME,
			"newRect", CONSTRUCTOR_METHOD_NAME,
			INSTANCE_GET_TYPE, getType,
			INSTANCE_GET_POS_X, xPosition,
			INSTANCE_GET_POS_Y, yPosition,
			INSTANCE_GET_RADIUS, radius,
			INSTANCE_GET_SIZE_Y, sizeY,
			INSTANCE_GET_SIZE_X, sizeX,
			nullptr, nullptr
		};

		void registerHitbox2D(lua_State* lua) {
			luaL_newmetatable(lua, INSTANCE_NAME_INHERITABLE);
			luaL_setfuncs(lua, functions, 0);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");
			lua_setglobal(lua, INSTANCE_NAME);
				
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
#define CONSTRUCTOR_METHOD_NAME newSprite
		constexpr auto INSTANCE_NAME_INHERITABLE = "Ingenium.Sprite";
		constexpr auto INSTANCE_NAME = "Sprite";

		/////////////////////////////////	Instance-specific methods
		/////////////////////////////////



		/////////////////////////////////
		/////////////////////////////////

		int FREE(lua_State* lua) {
			memory::safe_delete(*(Sprite**)lua_touserdata(lua, 1));
			return 0;
		}

		int toString(lua_State* lua) {
			Sprite* sp = uDataToPtr<Sprite>(lua_touserdata(lua, 1));
			lua_pushstring(lua, "sprite");
			return 1;
		}

		int CONSTRUCTOR_METHOD_NAME (lua_State* lua) {

			// name, path, vec2_image_size --- 3
			// name, path, vec2_image_size, vec2_frame_size, frames, frameTime, spriteSheetDirection --- 7
			// name, path, vec2_image_size, vec2_frame_size, frames, frameTime, spriteSheetDirection, callback_function, callback_function_frame --- 9

			int nargs = lua_gettop(lua);
			if (nargs != 3 && nargs != 7 && nargs != 9)
				return luaL_error(lua, "Got %d arguments, expected 3, 7, or 9.", nargs);
			if (lua_type(lua, 1) != LUA_TSTRING)
				return luaL_error(lua, "Argument 1 must be a string.");
			if (lua_type(lua, 1) != LUA_TSTRING)
				return luaL_error(lua, "Argument 2 must be a string.");

			Sprite** sp = (Sprite**)lua_newuserdata(lua, sizeof(Sprite*));

			switch (nargs) {
			case 3:
			{
				Sprite::FrameData fd = Sprite::FrameData();
				
				*sp = new Sprite(lua_tostring(lua, 1), (LPCWSTR) lua_tostring(lua, 2), fd, Engine::getEngine()->drwn->pRT);
				lua_remove(lua, -2);
				lua_remove(lua, -2);
				break;
			}
			case 7:
			{
				Hitbox2D* hb2d = uDataToPtr<Hitbox2D>(lua_touserdata(lua, 3));
				Sprite::FrameData fd = Sprite::FrameData();
				*sp = new Sprite(lua_tostring(lua, 1), (LPCWSTR) lua_tostring(lua, 2), fd, Engine::getEngine()->drwn->pRT);
				lua_remove(lua, -2);
				lua_remove(lua, -2);
				lua_remove(lua, -2);
				break;
			}
			default:
				break;
			}

			luaL_newmetatable(lua, INSTANCE_NAME_INHERITABLE);
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
			"new", CONSTRUCTOR_METHOD_NAME,
			nullptr, nullptr
		};

		void registerSprite(lua_State* lua) {
			luaL_newmetatable(lua, INSTANCE_NAME_INHERITABLE);
			luaL_setfuncs(lua, functions, 0);
			lua_pushvalue(lua, -1);
			lua_setfield(lua, -2, "__index");
			luaL_newlib(lua, functions, INSTANCE_NAME);
		}
#ifdef CONSTRUCTOR_METHOD_NAME
#undef CONSTRUCTOR_METHOD_NAME
#endif
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