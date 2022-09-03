#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "Debug/Debug.h"

class Window
{

public:

//=============================================================================================================================================//


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		  CONSTRUCTORS 			|
		|_______________________________|	*/

	//Creates and sets up the window.
	Window(const std::string title, unsigned const int width, unsigned const int height);


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		WRAPPER FUNCTIONS 		|
		|_______________________________|	*/

		//Returns a non-zero value as long the window is running.
	int is_running();


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

	//Returns the raw GLFW window pointer.
	GLFWwindow* getRawWindow() const;

	//Returns window height.
	int getHeight();
	
	//Returns window width.
	int getWidth();

	void addResizeCallback(std::function<void(int, int)> callback);

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
	void setup(const std::string title, unsigned const int width, unsigned const int height);


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
	GLFWwindow* window;


public:
	inline static auto CR = DY::ClassRegister<
		&is_running,
		&getRawWindow,
		&getHeight,
		&addResizeCallback,
		&setup>(
			"is_running",
			"getRawWindow",
			"getHeight",
			"addResizeCallback",
			"framebuffer_size");

	DY::ObjectRegister<Window, decltype(window)> OR;

	inline static auto FR = DY::FunctionRegister<
		&framebuffer_size_callback,
		&focus_callback,
		&iconify_callback>(
			"framebuffer_size_callback",
			"getRawWindow",
			"iconify_callback");

	inline static auto CB = DY::ClassBinder(CR);
	inline static auto FB = DY::FunctionBinder(FR);
	inline static auto OB = DY::ObjectBinder<decltype(OR)>();

	using LOG = _LOG<decltype(CB), decltype(OB), decltype(FB), DY::No_VB>;
};

