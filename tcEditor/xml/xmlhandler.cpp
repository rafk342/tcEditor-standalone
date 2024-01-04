#include "xmlhandler.h"


std::string xml_handler::root_name;
std::string xml_handler::cycle_name;
std::string xml_handler::regions_count;


vector<raw_timecycle_item> xml_handler::load_xml_to_arr(string& path) 
{
    
    vector<raw_timecycle_item> raw_items_array;
    
    raw_items_array.reserve(900);

    pugi::xml_document doc;
    pugi::xml_parse_result res = doc.load_file(path.c_str());

    if (!res) 
    {
        return raw_items_array;
    }

    pugi::xml_node root = doc.first_child();

    if (static_cast<std::string>(root.name()) != "timecycle_keyframe_data")
    {
        return raw_items_array;
    }
        
    root_name = root.name();
    cycle_name = root.child("cycle").attribute("name").value();
    regions_count = root.child("cycle").attribute("regions").value();


    for (const auto& cycle : root.children("cycle"))
    {
        for (const auto& region : cycle.children("region"))
        {
            for (const auto& child : region.children())
            {
                raw_items_array.push_back(raw_timecycle_item(child.name(), region.attribute("name").value(), child.text().get()));
            }
        }
    }

    return raw_items_array;
}


void xml_handler::load_arr_to_xml(string& path , timecycle_handler& t_handler)
{
    auto& names_ref = t_handler.timecycle_container.get_t_names();
    auto& t_map = t_handler.timecycle_container.get_t_map();

    pugi::xml_document doc;

    pugi::xml_node decl = doc.append_child(pugi::node_declaration);
    decl.append_attribute("version") = "1.0";
    decl.append_attribute("encoding") = "UTF-8";

    pugi::xml_node timecycle_keyframe_data = doc.append_child(root_name.c_str());
    timecycle_keyframe_data.append_attribute("version") = "1.000000";

    pugi::xml_node cycle = timecycle_keyframe_data.append_child("cycle");
    
    cycle.append_attribute("name") = cycle_name.c_str();
    cycle.append_attribute("regions") = "2";


    //////map = region --> names vector
    ////std::unordered_map<string, vector<string>> timecycle_item_names;

    //////map = region --> name --> timecycle_item
    ////std::unordered_map<string, std::unordered_map<string, timecycle_item>>timecycle_map;


    for (auto& [region , names_arr] : names_ref)
    {
        pugi::xml_node region_node = cycle.append_child("region");
        region_node.append_attribute("name") = region.c_str();

        for (string& name : names_arr) 
        {
            timecycle_item& t_item = t_map[region][name];

            if (t_item.type == timecycle_item_type::rgb)
            {
                region_node.append_child((name + "_r").c_str()).text() = convert_float_arr_to_str(t_item.r_params).c_str();
                region_node.append_child((name + "_g").c_str()).text() = convert_float_arr_to_str(t_item.g_params).c_str();
                region_node.append_child((name + "_b").c_str()).text() = convert_float_arr_to_str(t_item.b_params).c_str();
            }
            if (t_item.type == timecycle_item_type::values)
            {
                region_node.append_child((name).c_str()).text() = convert_float_arr_to_str(t_item.value_params).c_str();

            }
        }
    }
    doc.save_file(path.c_str());
}
