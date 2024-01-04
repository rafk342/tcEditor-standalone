
#include "gui_handler.h"

#define debug

gui_builder::gui_builder()
{
    strcpy_s(buff, config_params::path_from_cfg.c_str());
    time_array = config_params::cfg_time_values;
}




void gui_builder::file_section()
{
    ImGui::InputText("path", buff, sizeof(buff));

    if (ImGui::Button("Check files"))
    {
        check_files_btn_foo(buff);
    }

    ImGui::BeginChild("File_sel_window", ImVec2(-FLT_MIN, 130), ImGuiChildFlags_Border | ImGuiChildFlags_AutoResizeY);

    if (check_if_check_files_btn_was_pressed)
    {
        fill_file_sel_window();
    }

    ImGui::EndChild();

    if (ImGui::Button("Load selected"))
    {
        if (check_if_files_list_was_loaded)
        {
            load_file_to_arr();
            load_selected_check_for_categories_handling = true;

        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Save"))
    {
        if (check_if_file_was_loaded)
        {
            save_btn_foo();
        }
    }

    ImGui::SameLine();


    if (ImGui::Button("Save as"))
    {
        if (check_if_file_was_loaded)
        {
            save_as_btn_foo();
        }
    }

}



void gui_builder::main_params_window()
{
    static auto& ref_t_names = m_timecycle_handler.timecycle_container.get_t_names();
    static auto& ref_t_map = m_timecycle_handler.timecycle_container.get_t_map();
    static auto& t_regions = m_timecycle_handler.timecycle_container.get_regions();
    

    static std::vector<const char*> temp_regions = convert_str_to_char_vec(t_regions);
    static size_t regions_size = temp_regions.size();
     


    ImGui::Combo("regions", &current_region_index, temp_regions.data(), regions_size);
    ImGui::SameLine();
    ImGui::Checkbox("Edit both regions", &edit_both_regions);

    ImGui::BeginChild("Main_params_window", ImVec2(-FLT_MIN, 0.0), ImGuiChildFlags_Border);


    for (auto& name : ref_t_names[temp_regions[current_region_index]])
    {
        item_tree_node_type_selection(name, t_regions[current_region_index]);
    }


    ImGui::EndChild();
}







void gui_builder::categories_main_params_window()
{
    //category name --> tc item names
    static auto& ref_categories_names = CategoriesHandler::get_categories_names();
    static auto& ref_categories_map = CategoriesHandler::get_categories_map();

    static auto& ref_t_names = m_timecycle_handler.timecycle_container.get_t_names();
    static auto& ref_t_map = m_timecycle_handler.timecycle_container.get_t_map();
    static auto& t_regions = m_timecycle_handler.timecycle_container.get_regions();

    static std::unordered_map<std::string, std::unordered_map<std::string, int>>* categories_count = nullptr;


    static std::vector<const char*> temp_regions = convert_str_to_char_vec(t_regions);
    static size_t regions_size = temp_regions.size();

    auto& current_region_map = ref_t_map.at(t_regions[current_region_index]);


    switch (load_selected_check_for_categories_handling)
    {
    case(true):

        find_names_which_are_not_in_categories(ref_categories_map, ref_t_map, ref_categories_names);
        categories_count = &get_items_count(ref_categories_map, ref_t_map);
        load_selected_check_for_categories_handling = false;

        break;
    }


    ImGui::Combo("regions", &current_region_index, temp_regions.data(), temp_regions.size());
    ImGui::SameLine();
    
   
    ImGui::Checkbox("Edit both regions", &edit_both_regions);
    //if (ImGui::BeginItemTooltip())
    //{
    //    ImGui::Text("This will work just if you change the value");
    //    ImGui::EndTooltip();
    //}

    
    ImGui::BeginChild("Main_params_window", ImVec2(-FLT_MIN, 0.0), ImGuiChildFlags_Border);

    for (auto& category_name : ref_categories_names)
    {
        switch (categories_count->at(t_regions[current_region_index]).at(category_name))
        {
        case(0):
            continue;
            break;
        }

        switch (ImGui::CollapsingHeader(category_name.c_str()))
        {
        case(true):

            for (auto& name : ref_categories_map.at(category_name))
            {
                switch (current_region_map.contains(name))
                {
                case(true):

                    item_tree_node_type_selection(name, t_regions[current_region_index]);

                    break;
                }
            }
            break;
        }
    }
    ImGui::EndChild();
}




void gui_builder::item_tree_node_type_selection(std::string& name, std::string& region)
{
    if (config_params::tooltips)
    {
        make_tree_node_with_tooltip(name, region);
    }
    else if (config_params::replace_item_names_with_tooltips_definition)
    {
        make_tree_node_with_tooltip_names(name, region);
    }
    else
    {
        make_tree_node(name, region);
    }
}





void gui_builder::HelpMarker(std::string& tc_item_name, int num)
{
    static auto& ref_tooltips = TooltipsHandler::get_tooltip_map();

    if (!ref_tooltips.contains(tc_item_name))
    {
        return;
    }

    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(("\n  " + ref_tooltips.at(tc_item_name) + "  \n" + ' ').c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }

}




void gui_builder::make_tree_node_with_tooltip_names(std::string& name, std::string& current_region)
{
    static auto& tooltip_map = TooltipsHandler::get_tooltip_map();

    //ImGui::SetNextItemOpen(1);

    if (tooltip_map.contains(name))
    {
        if (ImGui::TreeNode(tooltip_map.at(name).c_str()))
        {


            make_table(current_region, name);
            ImGui::TreePop();
        }
    }
    else
    {
        if (ImGui::TreeNode(name.c_str()))
        {
            make_table(current_region, name);
            ImGui::TreePop();
        }
    }

}




void gui_builder::make_tree_node(std::string& name, std::string& current_region)
{
    if (ImGui::TreeNode(name.c_str()))
    {
        make_table(current_region, name);
        ImGui::TreePop();
    }
}




void gui_builder::make_tree_node_with_tooltip(std::string& name, std::string& current_region)
{
    if (ImGui::TreeNode(name.c_str()))
    {
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))      HelpMarker(name, 1);

        make_table(current_region, name);
        ImGui::TreePop();
    }
    else
    {
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_Stationary))      HelpMarker(name, 1);
    }
}





void gui_builder::make_table(const std::string &region, std::string &name) {

    static auto& ref_t_map = m_timecycle_handler.timecycle_container.get_t_map();
    static auto& ref_t_regions = m_timecycle_handler.timecycle_container.get_regions();
    static timecycle_item *other_item = nullptr;



//                  inverting the current_region_index is a bit wrong,
//                  i hope no one will load timecycle here with more that 2 regions 

    if (ref_t_map.at(ref_t_regions[!current_region_index]).contains(name))
    {
        other_item = &ref_t_map.at(ref_t_regions[!current_region_index]).at(name);
    }
    else
    {
        other_item = nullptr;
    }

    auto& item = ref_t_map.at(region).at(name);

    static ImVec4 color;
    static ImVec4 other_color;

    static string* label_name;

    ImVec2 tableSize = ImGui::GetContentRegionAvail();

    if (ImGui::BeginTable(name.c_str() , time_array.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_ContextMenuInBody))
    {
        for (size_t i = 0; i < 2; i++)
        {
            ImGui::TableNextRow();

            for (size_t j = 0; j < time_array.size(); j++)
            {
                ImGui::TableNextColumn();

                switch (i)
                {
                case 0:
                    ImGui::Text(time_array[j].c_str());
                    break;
                case 1:
                    switch (item.type)
                    {
                    case timecycle_item_type::rgb:

                        color.x = item.r_params[j];
                        color.y = item.g_params[j];
                        color.z = item.b_params[j];
                       
                        label_name = &item.label_name;

                        if (ImGui::ColorEdit3((*label_name + std::to_string(j)).c_str(), (float*)&color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | /*ImGuiColorEditFlags_NoTooltip |*/ ImGuiColorEditFlags_NoBorder))
                        {
                            item.r_params[j] = color.x;
                            item.g_params[j] = color.y;
                            item.b_params[j] = color.z;

                            if (edit_both_regions && other_item != nullptr)
                            {
                                other_item->r_params[j] = color.x;
                                other_item->g_params[j] = color.y;
                                other_item->b_params[j] = color.z;
                            }
                        }

                        break;

                    case timecycle_item_type::values:

                        label_name = &item.label_name;

                        ImGui::SetNextItemWidth(tableSize.x / 13);
                        if (ImGui::DragFloat((*label_name + std::to_string(j)).c_str(), &item.value_params[j], 0.01f))
                        {
                            if (edit_both_regions && other_item != nullptr)
                            {
                                other_item->value_params[j] = item.value_params[j];
                            }
                        }

                        break;
                    }
                    break;
                }
            }
        }
    }
    ImGui::EndTable();
}



std::wstring stringToWideString(const std::string& str) 
{
    std::wstring wideStr(str.begin(), str.end());
    return wideStr;
}






std::string gui_builder::selectPathForSaveAsBtn() 
{
    std::wstring widePath = stringToWideString(this->loaded_file_path);

    wchar_t initialDir[MAX_PATH];
    wcscpy_s(initialDir, widePath.c_str());

    OPENFILENAMEW ofn;                                      //doubtfully
    ZeroMemory(&ofn, sizeof(ofn));                          //doesn't really work as it should
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = filename;
    ofn.lpstrFile[0] = L'\0';
    ofn.nMaxFile = sizeof(filename) / sizeof(wchar_t);
    ofn.lpstrFilter = L"XML(*.xml)\0*.xml\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrTitle = L"Save File";
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

    size_t pos = loaded_file_path.find('\\');
    if (pos != std::string::npos) 
    {
        std::wstring disk = stringToWideString(this->loaded_file_path.substr(0, pos + 1));
        ofn.lpstrInitialDir = disk.c_str();
        ofn.lpstrInitialDir = initialDir;
    }
    if (GetSaveFileNameW(&ofn)) 
    {
        std::wstring ws(filename);
        string str(ws.begin(), ws.end());

        if (!check_str_end(str, ".xml"))
            str += ".xml";
        return str;
    }
    return "";
}




bool gui_builder::get_file_load_state()
{
    return this->check_if_file_was_loaded;
}



void gui_builder::save_btn_foo()
{
    static auto& ref_t_map = m_timecycle_handler.timecycle_container.get_t_map();


    if (ref_t_map.size() > 0)
        xml_handler::load_arr_to_xml(loaded_file_path, m_timecycle_handler);
}





void gui_builder::save_as_btn_foo()
{
    static string path_to_save;
    path_to_save = selectPathForSaveAsBtn();

    if (path_to_save.size() != 0)
        xml_handler::load_arr_to_xml(path_to_save, m_timecycle_handler);
}



void gui_builder::load_file_to_arr()
{

    static std::unordered_map<std::string, std::string>& files_map = fhandler.get_files_map();
    static std::string selected_name;

    if (files_map.size() == 0)
    {
        return;
    }


    for (auto& [bname, bvalue] : files_bool)
    {
        if (bvalue)
        {
            selected_name = bname;
        }
    }

    auto raw_items_array = xml_handler::load_xml_to_arr(files_map.at(selected_name));

    if (raw_items_array.empty())
    {
        return;
    }

    m_timecycle_handler.timecycle_container.fill_container(raw_items_array);
    loaded_file_path = files_map.at(selected_name);

    if (m_timecycle_handler.timecycle_container.get_container_size() != 0)
        check_if_file_was_loaded = true;
}


void gui_builder::check_files_btn_foo(char* buf)
{
    this->path.assign(buf);
    this->fhandler.set_files_map(this->path, ".xml");

    files_bool.clear();
    check_if_check_files_btn_was_pressed = true;
    files_bool_filled_check = false;
}


void gui_builder::fill_file_sel_window()
{
    static std::unordered_map<std::string, std::string>& files_map = fhandler.get_files_map();
    static std::string name;

    if (fhandler.invalid_path_check())
    {
        ImGui::Text("Invalid directory path");
        return;
    }

    if (files_map.empty())
    {
        ImGui::Text("No files found");
        return;
    }

    if (!files_bool_filled_check) 
    {
        bool first = true;

        for (const auto& file : files_map) 
        {
            if (first)
            {
                files_bool[file.first] = first;
                first = !first;
            } else {
                files_bool[file.first] = first;
            }
        }
        files_bool_filled_check = true;
    }

    for (auto& file : files_map) 
    {
        const std::string& name = file.first;

        if (ImGui::RadioButton(name.c_str(), files_bool.at(name))) 
        {
            for (auto& [file_name, bool_val] : files_bool) 
            {
                bool_val = false;
            }
            files_bool.at(name) = true;
        }
    }
    check_if_files_list_was_loaded = (files_map.size() != 0);
}







void gui_builder::HelpMarker(std::string& tc_item_name)
{

    if (!config_params::tooltips)
    {
        return;
    }

    static auto& ref_tooltips = TooltipsHandler::get_tooltip_map();

    if (!ref_tooltips.contains(tc_item_name))
    {
        return;
    }

    ImGui::TextDisabled(" (?) ");

    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(('\n' + ref_tooltips.at(tc_item_name) + '\n' + ' ').c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
    
    ImGui::SameLine();
}



















void find_names_which_are_not_in_categories(
    //category name --> item names vec
    std::unordered_map<std::string, std::vector <std::string> >& ref_categories_map,
    //map = region --> name --> timecycle_item
    std::unordered_map<string, std::unordered_map<string, timecycle_item>>& ref_t_map,

    std::vector<std::string>& ref_categories_names)
{

    std::vector<std::string> temp_vec;
    temp_vec.clear();
    
    bool found = false;
    std::string none_name = "None";
    std::string temp1;

    for (auto& [region, region_map] : ref_t_map)
    {
        for (auto& [tc_name, item] : region_map)
        {
            found = false;

            for (auto& [category_name, vec] : ref_categories_map)
            {
                for (auto& c_name : vec)
                {
                    if (tc_name == c_name)
                    {
                        found = true;
                        break;
                    }
                }

                if (found)
                {
                    break;
                }
            }
            if (!found) {
                temp_vec.push_back(tc_name);
            }
        }
    }

    bool check_if_none_in_vec = false;

    for (size_t i = 0; i < ref_categories_names.size(); i++)
    {
        if (ref_categories_names[i] == none_name)
        {
            check_if_none_in_vec = true;
            break;
        }
    }

    if (!temp_vec.empty())
    {
        RemoveDuplicatesInVector(temp_vec);

        std::sort(temp_vec.begin(), temp_vec.end());

        if (!check_if_none_in_vec)
        {
            ref_categories_names.push_back(none_name);
        }
        ref_categories_map[none_name].insert(ref_categories_map[none_name].end(), temp_vec.begin(), temp_vec.end());
    }
}






std::unordered_map<std::string, std::unordered_map<std::string, int>>& get_items_count(
    //category name --> item names vec
    std::unordered_map<std::string, std::vector <std::string> >& ref_categories_map,
    //map = region --> name --> timecycle_item
    std::unordered_map<string, std::unordered_map<string, timecycle_item>>& ref_t_map)
{
    //region -> category -> count
    static std::unordered_map<std::string, std::unordered_map<std::string, int>> items_count;
    items_count.clear();

    for (const auto& [region, name] : ref_t_map)
    {
        for (const auto& [category_name, names_vec] : ref_categories_map)
        {
            items_count[region][category_name] = 0;
        }
    }

    std::string category;

    for (const auto& [region, timecycle_map] : ref_t_map)
    {
        for (const auto& [name, item] : timecycle_map)
        {
            for (const auto& [category_name, names_vec] : ref_categories_map)
            {
                if (std::find(names_vec.begin(), names_vec.end(), name) != names_vec.end())
                {
                    items_count[region][category_name]++;
                    break;
                }
            }
        }
    }

    return items_count;
}
