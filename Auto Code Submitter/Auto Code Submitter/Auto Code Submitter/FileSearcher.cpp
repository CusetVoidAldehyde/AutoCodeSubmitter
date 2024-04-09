#include "FileSearcher.h"
#include <iostream>
#include <vector>
#include <io.h>

void FileSearcher::getNeedFile(std::string path, std::vector<std::string>& file, std::string ext) {
	intptr_t file_handle = 0;
	struct _finddata_t file_info;
	std::string temp;
	if ((file_handle = _findfirst(temp.assign(path).append("/*" + ext).c_str(), &file_info)) != -1)
	{
		do
		{
			file.push_back(temp.assign(path).append("/").append(file_info.name));
		} while (_findnext(file_handle, &file_info) == 0);
		_findclose(file_handle);
	}
}

void FileSearcher::traversalFiles(std::string file_path) {
	std::vector<std::string> my_file;
	std::string need_extension = ".bmp";
	getNeedFile(file_path, my_file, need_extension);
	for (int i = 0; i < my_file.size(); i++)
	{
		std::cout << "File " << i + 1 << " is:\n";
		std::cout << my_file[i] << "\n";
	}
	if (my_file.size() == 0)
	{
		std::cout << "No file can be found!\n";
	}
	else
	{
		std::cout << "\n" << "Find " << my_file.size() << " file(s).\n";
	}
}