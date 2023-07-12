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

		std::streamsize size = getFileSize(file);
		if (size != -1) {
			std::string result;
			result.resize(size);
			file.seekg(0, std::ifstream::beg);
			file.read(result.data(), size);
			return result;
		}
		KYSE_ASSERT(size != -1, std::format("Could not read file: {}", path));
		return "";
	}

	static std::streamsize getFileSize(std::ifstream& file) {
		file.seekg(0, std::ifstream::end);
		std::streamsize size = file.tellg();
		return size;
	}

	static bool validate(const std::string& path) {
		std::ifstream file = openFile(path);
		std::streamsize size = getFileSize(file);
		KYSE_ASSERT(size != -1, std::format("Could not read file: {}", path));
		return size;
	}

	static std::ifstream openFile(const std::string& path) {
		return std::ifstream(path, std::ifstream::in | std::ifstream::binary);
	}
};

