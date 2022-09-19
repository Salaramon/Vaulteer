#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Debug/DebugLogger.h"


static size_t GLFWWindowCount = 0;

class Window : public DebugLogger<Window> {

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


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	//Returns the raw GLFW window pointer
	static GLFWwindow* getRawWindow();

	static int getWidth();
	static int getHeight();

	static void addResizeCallback(const std::function<void(int, int)>& callback);

	//Displays a completed frame
	static bool onUpdate();

private:
	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		CALLBACK FUNCTIONS 		|
		|_______________________________|	*/

	//Is called when any windows' size is changed to update window parameters
	static void framebufferSizeCallback(GLFWwindow*, int width, int height);

	static void focusCallback(GLFWwindow*, int focused);

	static void iconifyCallback(GLFWwindow*, int iconified);

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
};
