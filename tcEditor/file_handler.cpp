#include "file_handler.h"


void file_handler::set_files_map(std::filesystem::path path, std::string extension)
{
    
    files.clear();

    namespace stdfs = std::filesystem;

    if (!stdfs::exists(path) || !stdfs::is_directory(path)) 
    {
        invalid_path = true;
        return;
    }

    const stdfs::directory_iterator end{};

    for (const auto& entry : stdfs::directory_iterator{path})
    {
        if (stdfs::is_regular_file(entry) && entry.path().extension() == extension)
        {
            files.insert({ entry.path().filename().string(), entry.path().string() });
        }
    }

    invalid_path = false;

}

std::unordered_map<std::string, std::string>& file_handler::get_files_map()
{
    return files;
}


bool file_handler::invalid_path_check()
{
    return invalid_path;
}




std::map<std::string, std::string> file_handler::_get_files_map(std::filesystem::path path, std::string extension)
{

    std::map<std::string, std::string> files0;

    namespace stdfs = std::filesystem;

    if (!stdfs::exists(path) || !stdfs::is_directory(path))
    {
        return files0;
    }

    const stdfs::directory_iterator end{};

    for (const auto& entry : stdfs::directory_iterator{ path })
    {
        if (stdfs::is_regular_file(entry) && entry.path().extension() == extension)
        {
            files0.insert({ entry.path().filename().string(), entry.path().string() });
        }
    }


    return files0;

}