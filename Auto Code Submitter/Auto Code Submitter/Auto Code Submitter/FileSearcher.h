#pragma once
#include <string>
#include <cstring>
#include <vector>

class FileSearcher {
public:
	void getNeedFile(std::string path, std::vector<std::string>& file, std::string ext);
	void traversalFiles(std::string file_path);
};