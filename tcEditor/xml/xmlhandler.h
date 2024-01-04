#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "../timecycle_item.h"
#include "pugixml.hpp"
#include "../timecycle_handler.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

class xml_handler
{
	 static string root_name;
	 static string cycle_name;
	 static string regions_count;
	
public:
	 static vector<raw_timecycle_item> load_xml_to_arr(string& path);
	 static void load_arr_to_xml(string& path, timecycle_handler& t_handler);
};

