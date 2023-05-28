#pragma once

#include <string>
#include <iostream>

//Takes file contents and converts it to string.
//Returns contents of a file in string format.
inline std::string readFile(std::string& path) {
	// TODO read without exceptions...
	std::ifstream file;
	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		std::stringstream stream;

		file.open(path);
		stream << file.rdbuf();
		file.close();
		return stream.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << std::format("Error reading file:\n\t{}", path) << std::endl;
		__debugbreak();
		//Return empty string upon error.
		return "";
	}	
}


inline std::ifstream openFile(const std::string& path) {
	// TODO handle errors cuz fuckyou
	return std::ifstream(path);
}


