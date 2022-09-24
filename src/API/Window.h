#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Debug/DebugLogger.h"

class Window : public DebugLogger<Window> {

public:
	//=============================================================================================================================================//


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		  CONSTRUCTORS 			|
		|_______________________________|	*/

	//Creates and sets up the window.
	Window(const std::string& title, unsigned int width, unsigned int height);


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		WRAPPER FUNCTIONS 		|
		|_______________________________|	*/

	//Returns a non-zero value as long the window is running.
	int isRunning() const;


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	//Returns the raw GLFW window pointer.
	static GLFWwindow* getRawWindow();

	//Returns window height.
	int getHeight() const;

	//Returns window width.
	int getWidth() const;

	void addResizeCallback(const std::function<void(int, int)>& callback) const;

private:
	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		CALLBACK FUNCTIONS 		|
		|_______________________________|	*/

	//Is called when any windows' size is changed to update window parameters.
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	static void focus_callback(GLFWwindow* window, int focused);

	static void iconify_callback(GLFWwindow* window, int iconified);

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		 CLASS FUNCTIONS 		|
		|_______________________________|	*/

	//Initializes the window.
	void setup(const std::string& title, unsigned int width, unsigned int height);


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	inline static std::unordered_map<GLFWwindow*, std::vector<std::function<void(int, int)>>> resizeCallbacks;

protected:
	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|			VARIABLES			|
		|_______________________________|	*/

	//GLFW window variable.
	inline static GLFWwindow* window = nullptr;
};
