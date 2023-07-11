#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <format>

class FileSystem {
public:

	//Takes file contents and converts it to string.
	//Returns contents of a file in string format.
	static std::string readFileToString(const std::string& path) {
		std::ifstream file(path, std::ifstream::in | std::ifstream::binary);

		file.seekg(0, std::ifstream::end);
		std::streamsize size = file.tellg();

		if (size != -1) {
			std::string result;
			result.resize(size);
			file.seekg(0, std::ifstream::beg);
			file.read(result.data(), size);
			return result;
		}
		
		std::cout << std::format("Error reading file:\n\t{}", path) << std::endl;
		__debugbreak();
		return "";
	}

	// opens file for reading
	static std::ifstream openFile(const std::string& path) {
		// TODO handle errors cuz fuckyou
		return std::ifstream(path, std::ifstream::in | std::ifstream::binary);
	}
};

