#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include "timecycle_item.h"
#include "helpers.h"



using std::string;
using std::vector;
using std::cout;
using std::endl;


class tc_helper
{
	int find_item_by_suff(const vector<raw_timecycle_item>& raw_timecycle_array, const string& item_col_name, string& region, string suff);
public:
	std::array<raw_timecycle_item*, 2>& get_rgb_items(std::vector<raw_timecycle_item>& raw_timecycle_array, string& item_name, string& region);

};



class timecycle_container 
{
	tc_helper tcHelper;
	
	//map = region --> names vec
	std::unordered_map<string, vector<string>> timecycle_item_names;

	//map = region --> name --> timecycle_item
	std::unordered_map<string, std::unordered_map<string , timecycle_item>>timecycle_map;
	std::vector<string> regions;

	void set_regions(const vector<raw_timecycle_item>& raw_timecycle_array);

public:

	std::unordered_map<std::string, std::vector<std::string>>& get_t_names();
	std::unordered_map<string, std::unordered_map<string, timecycle_item>>& get_t_map();
	std::vector<string>& get_regions();

	void fill_container(vector<raw_timecycle_item>& raw_timecycle_array);
	int get_container_size();

};


class timecycle_handler
{
public:

	timecycle_container timecycle_container;

};


