#include "timecycle_handler.h"


int tc_helper::find_item_by_suff(const vector<raw_timecycle_item>& raw_timecycle_array, const string& item_col_name, string& region, string suff) {

	int index = 0;
	for (auto& item_check : raw_timecycle_array) 
	{
		if (remove_last_2_symb(item_check.name) == item_col_name && check_str_end(item_check.name, suff) && item_check.region == region) 
		{
			return index;
		}
		index++;
	}
	return -1;
}


std::array<raw_timecycle_item*, 2>& tc_helper::get_rgb_items(std::vector<raw_timecycle_item>& raw_timecycle_array, string& item_name, string& region) {
	int b_index;
	int g_index;

	bool found = false;

	static std::array<raw_timecycle_item*, 2> temp_rgb_ptr_arr { nullptr, nullptr };

	g_index = tc_helper::find_item_by_suff(raw_timecycle_array, item_name, region, "_g");
	b_index = tc_helper::find_item_by_suff(raw_timecycle_array, item_name, region, "_b");

	if (g_index != -1) temp_rgb_ptr_arr[0] = &raw_timecycle_array[g_index];
	if (b_index != -1) temp_rgb_ptr_arr[1] = &raw_timecycle_array[b_index];

	return temp_rgb_ptr_arr;
}



void timecycle_container::set_regions(const vector<raw_timecycle_item>& raw_timecycle_array)
{
	std::unordered_set<string> regions;
	this->regions.clear();

	for (auto& item : raw_timecycle_array) 
	{
		regions.insert(item.region);
	}

	for (auto& region : regions)
	{
		this->regions.push_back(region);
	}
}	



void timecycle_container::fill_container(vector<raw_timecycle_item>& raw_timecycle_array) 
{

	string item_col_name;

	if (raw_timecycle_array.size() == 0)
		return;

	this->timecycle_item_names.clear();
	this->timecycle_map.clear();
	this->set_regions(raw_timecycle_array);

	for (auto& region : this->regions)
	{
		for (auto& item : raw_timecycle_array)
		{
			if (item.region == region) 
			{
				if (check_str_end(item.name, "_r")) 
				{
					item_col_name = remove_last_2_symb(item.name);

					std::array<raw_timecycle_item*, 2>& gbItems = (tcHelper.get_rgb_items(raw_timecycle_array, item_col_name, item.region));
					
					auto& inner_map = timecycle_map[item.region];

					timecycle_item_names[item.region].push_back(item_col_name);

					inner_map.emplace(item_col_name, timecycle_item(item_col_name, item.region, item.params, gbItems[0]->params, gbItems[1]->params));
				}

				else if (check_str_end(item.name, "_g") || check_str_end(item.name, "_b")) 
				{
					continue;
				}

				else
				{
					auto& inner_map = timecycle_map[item.region];
					timecycle_item_names[item.region].push_back(item.name);

					inner_map.emplace(item.name, timecycle_item(item.name, item.region, item.params));
				}
			}
		}
	}
	for (auto& region : timecycle_item_names) 
		region.second.erase(std::unique(region.second.begin(), region.second.end()), region.second.end());

}

int timecycle_container::get_container_size()
{
	return this->timecycle_map.size();
}


std::unordered_map<std::string, std::vector<std::string>>& timecycle_container::get_t_names()
{
	return this->timecycle_item_names;
}

std::unordered_map<string, std::unordered_map<string, timecycle_item>>& timecycle_container::get_t_map()
{
	return this->timecycle_map;
}

std::vector<string>& timecycle_container::get_regions()
{
	return this->regions;
}
