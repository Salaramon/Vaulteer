﻿#pragma once

#include <glad/glad.h>
#include <glfw3.h>

#include <string>
#include <iostream>

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
	GLFWwindow* getRawWindow();

	//Returns window height.
	int getHeight();
	
	//Returns window width.
	int getWidth();


private:

	//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		CALLBACK FUNCTIONS 		|
		|_______________________________|	*/

	//Is called when any windows' size is changed to update window parameters.
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		 CLASS FUNCTIONS 		|
		|_______________________________|	*/

	//Initializes the window.
	void setup(const std::string title, unsigned const int width, unsigned const int height);


	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|		UTILITY FUNCTIONS 		|
		|_______________________________|	*/

protected:

//=============================================================================================================================================//

	/*	|‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|
		|			VARIABLES			|
		|_______________________________|	*/

	//GLFW window variable.
	GLFWwindow* window;
};

