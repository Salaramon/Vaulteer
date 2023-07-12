#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Core.h"
#include "Events/EventTypes.h"

static size_t GLFWWindowCount = 0;

struct WindowSpecification {
	std::string title;
	int width, height;
	int x, y;

	// can eventually put initial state here
};

class Window {
public:
	// Fields
	inline static WindowSpecification specification;

	inline static bool fullscreen = false;
	inline static bool maximized = false;
	inline static bool focused = true;

	inline static bool isClosing = false;

	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		  CONSTRUCTORS 			|
		|_______________________________|	*/

	//Creates and sets up the window
	Window(WindowSpecification spec);

	~Window();

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	//Returns the raw GLFW window pointer
	static GLFWwindow* getRawWindow();

	static int getWidth();
	static int getHeight();

	static int isRunning();

	static void addResizeCallback(const std::function<void(int, int)>& callback);

	//Displays a completed frame
	static bool onUpdate();

	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		CALLBACK FUNCTIONS 		|
		|_______________________________|	*/

	static bool onWindowCloseEvent(const WindowCloseEvent& e);
	static bool onWindowFullscreenEvent(const WindowFullscreenEvent& e);
	static bool onWindowMaximizeEvent(const WindowMaximizeEvent& e);
	static bool onWindowPositionEvent(const WindowPositionEvent& e);
	static bool onWindowFocusEvent(const WindowFocusEvent& e);
	static bool onWindowResizeEvent(const WindowResizeEvent& e); // calls all registered callbacks
	//static void onWindowIconifyCallback(const WindowIconifyEvent& e);
	

private:

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		 CLASS FUNCTIONS 		|
		|_______________________________|	*/

	//Initializes the window
	void setup();

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	inline static std::unordered_map<GLFWwindow*, std::vector<std::function<void(int, int)>>> resizeCallbacks;

	//=============================================================================================================================================//

	//GLFW internal window
	inline static GLFWwindow* window{};
};
