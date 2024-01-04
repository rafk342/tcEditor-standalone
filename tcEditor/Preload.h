#pragma once
#include <iostream>
#include "inih/INIReader.h"
#include <fstream>
#include <vector>
#include <unordered_map>

#include <array>
#include "helpers.h"
#include <regex>
#include <chrono>

///////////////////////////////////////////////////////////////////////////////////////////                    
//                               config





class configHandler
{
	static std::string config_name;
	static void WriteDefaultParamsToCfg();

public:

	static void readCfg();

};


struct config_params
{
	config_params() = default;

	static std::string path_from_cfg;
	static bool categories;
	static std::array<std::string, 13> cfg_time_values;
	static bool tooltips;
	static bool replace_item_names_with_tooltips_definition;


};


struct default_cfg_params
{

	static std::string default_path;
	static std::string default_time_values;
	const static int default_categories = 1;
	const static int default_tooltips = 0;
	const static int default_replace_item_names_with_tooltips_definition = 1;

};



std::array<std::string, 13> convert_time_str_to_arr(std::string time_str);





///////////////////////////////////////////////////////////////////////////////////////////                    
//                               categories






class CategoriesHandler 
{
	static void save_hardcoded_params_to_file();
	static void handle_file_params(std::ifstream& infile);
	static void remove_repeatings_from_map();
	
	static void load_hardcoded_categories();
	static void load_file_categories();

	static std::vector<std::string> category_names;

	//category name --> tc item names vec
	static std::unordered_map<std::string, std::vector<std::string>> categories_map;
	static std::string categories_filename;


public:

	static void LoadCategories();
	static std::vector<std::string>&	get_categories_names() { return category_names; }
	static std::unordered_map<std::string, std::vector<std::string>>&	get_categories_map() { return categories_map; }

};






/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////								Tooltips



class TooltipsHandler
{

	static void load_hardcoded_tooltips();
	static void write_tooltips_to_file();
	static void load_tooltips_from_file();
	static std::unordered_map<std::string, std::string> tooltips_map;
	static std::string tooltips_filename;
	static std::vector<std::string> tooltips_order;

public:

	static void load_tooltips();
	static std::unordered_map<std::string, std::string>& get_tooltip_map();

};