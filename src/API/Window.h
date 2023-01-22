#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Debug/Debug.h"
#include "Events/EventTypes.h"

static size_t GLFWWindowCount = 0;

class Window {
public:
	//=============================================================================================================================================//


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		  CONSTRUCTORS 			|
		|_______________________________|	*/

	//Creates and sets up the window
	Window(const std::string& title, unsigned int width, unsigned int height);

	~Window();


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		WRAPPER FUNCTIONS 		|
		|_______________________________|	*/

	//Returns a non-zero value as long the window is running
	static int isRunning();

	static bool isFocused();


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	//Returns the raw GLFW window pointer
	static GLFWwindow* getRawWindow();

	static int getWidth();
	static int getHeight();

	static void addResizeCallback(std::function<void(int, int)> callback);

	//Displays a completed frame
	static bool onUpdate();

	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		CALLBACK FUNCTIONS 		|
		|_______________________________|	*/

	static bool onWindowCloseEvent(const WindowCloseEvent& e);
	static bool onWindowFocusEvent(const WindowFocusEvent& e);
	static bool onWindowResizeEvent(const WindowResizeEvent& e); // calls all registered callbacks
	//static void onWindowIconifyCallback(const WindowIconifyEvent& e);
	

private:

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		 CLASS FUNCTIONS 		|
		|_______________________________|	*/

	//Initializes the window
	void setup(const std::string& title, int width, int height);

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	inline static std::unordered_map<GLFWwindow*, std::vector<std::function<void(int, int)>>> resizeCallbacks;

protected:
	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|			VARIABLES			|
		|_______________________________|	*/

	//GLFW window variable
	inline static GLFWwindow* window{};
	inline static bool focused = true;

public:
	inline static auto CR = DY::ClassRegister<
		&setup>(
			"setup");

	DY::ObjectRegister<Window, decltype(window)> OR;

	inline static auto FR = DY::FunctionRegister<
		&onWindowCloseEvent,
		&onWindowFocusEvent,
		&onWindowResizeEvent>(
			"onWindowCloseEvent",
			"onWindowFocusEvent",
			"onWindowResizeEvent");

	inline static auto CB = DY::ClassBinder(CR);
	inline static auto FB = DY::FunctionBinder(FR);
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<decltype(CB), decltype(OB), decltype(FB), DY::No_VB>;
};
