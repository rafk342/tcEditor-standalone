#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <unordered_map>
#include <map>


class file_handler
{

	//map filename -> path
	std::unordered_map<std::string, std::string> files;
	bool invalid_path;

public:

	void set_files_map(std::filesystem::path path, std::string extension);
	std::unordered_map<std::string, std::string>& get_files_map();
	bool invalid_path_check();

	std::map<std::string, std::string> _get_files_map(std::filesystem::path path, std::string extension);
};

