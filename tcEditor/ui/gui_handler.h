#pragma once

#include <iostream>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <string>
#include <array>
#include "../timecycle_item.h"
#include "../timecycle_handler.h"
#include "../file_handler.h"
#include "../xml/xmlhandler.h"
#include <vector>
#include <windows.h>
#include <Shlwapi.h>
#include "../Preload.h"
#include "../helpers.h"



class gui_builder
{
	
	char buff[255];

	std::string path;
	std::string selected_file;
	std::string loaded_file_path;

	std::array<string, 13> time_array;
	std::unordered_map<std::string, bool> files_bool;

	bool edit_both_regions = false;
	
	bool load_selected_check_for_categories_handling = false;
	bool check_if_files_list_was_loaded = false;
	bool check_if_check_files_btn_was_pressed = false;
	bool load_selected_btn_bool = false;
	bool files_bool_filled_check = false;
	bool check_if_file_was_loaded = false;

	timecycle_handler m_timecycle_handler;
	file_handler fhandler;
	
	int current_region_index = 0;


	wchar_t filename[MAX_PATH];
	
	void make_table(const std::string& region, std::string& name);
	void save_btn_foo();
	void save_as_btn_foo();
	void load_file_to_arr();
	void check_files_btn_foo(char* buf);
	void fill_file_sel_window();
	string selectPathForSaveAsBtn();
	void HelpMarker( std::string& tc_item_name);
	void HelpMarker(std::string& tc_item_name, int num);
	void make_tree_node_with_tooltip(std::string& name, std::string& current_region);
	void make_tree_node(std::string& name, std::string& current_region);
	void make_tree_node_with_tooltip_names(std::string& name, std::string& current_region);
	void item_tree_node_type_selection(std::string& name, std::string& region);



public:
	
	gui_builder();

	bool get_file_load_state();
	void file_section();
	void categories_main_params_window();
	void main_params_window();
	
};






///////////////////////////////////////////////////////////////////////////////////////////////////////////////			
//							It's something related to categories



void find_names_which_are_not_in_categories(std::unordered_map<std::string, std::vector <std::string> >& ref_categories_map,
											std::unordered_map<string, std::unordered_map<string, timecycle_item>>& ref_t_map,
											std::vector<std::string>& ref_categories_names);


std::unordered_map<std::string, std::unordered_map<std::string, int>>& get_items_count(
						std::unordered_map<std::string, std::vector <std::string> >& ref_categories_map,
						std::unordered_map<string, std::unordered_map<string, timecycle_item>>& ref_t_map);


