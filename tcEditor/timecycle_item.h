#pragma once
#include <array>
#include <string>
#include <unordered_map>
#include "helpers.h"

using std::string;



enum timecycle_item_type 
{
	rgb,
	values
};


struct raw_timecycle_item 
{

	raw_timecycle_item(string name, string region, string params) {
		this->name = name;
		this->region = region;
		this->params = params;
	}

	string name = "";
	string region = "";
	string params = "";

};



struct timecycle_item 
{
	
	timecycle_item() = default;

	timecycle_item(string name, string region, string& params) 
	{
		this->name = name;
		
		this->region = region;
		this->value_params = convert_str_to_float_arr(params, 13);
		
		this->type = timecycle_item_type::values;

		this->item_id = generate_id();

		this->label_name = "##" + region + name + std::to_string(item_id);

	}

	timecycle_item(string name, string region, string r_params, string g_params, string b_params) 
	{
		this->name = name;
		this->region = region;
		
		this->r_params = convert_str_to_float_arr(r_params, 13);
		this->g_params = convert_str_to_float_arr(g_params, 13);
		this->b_params = convert_str_to_float_arr(b_params, 13);
		
		this->type = timecycle_item_type::rgb;

		this->item_id = generate_id();
		this->label_name = "##" + region + name + std::to_string(item_id);
	}

	string name;
	string region;

	string label_name;

	timecycle_item_type type;
	
	vector<float> value_params;
	vector<float> r_params;
	vector<float> g_params;
	vector<float> b_params;
	
	int get_id() const{
		return this->item_id;
	}

private:
	static int id;
	static int generate_id() {
		return id++;
	}

	int item_id;

};

