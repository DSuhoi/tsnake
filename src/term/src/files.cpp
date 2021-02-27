#include <filesystem>
#include <fstream>
#include <string>
#include "files.hpp"

namespace fs = std::filesystem;

// Writing the game result to the file
void FileSystem::save_records(long *score)
{
    std::string path_str = getenv("HOME");
    path_str += MAIN_PATH_STR;
    path_str += FILE_NAME_SCORE_STR;
    fs::path file_path(path_str);
    
    std::ofstream fout(file_path.native(), std::ios::binary);
    for (unsigned int i = 0; i < 30; ++i)                // 3 map types * 10 speed level = 30 
        fout.write((char*)&score[i], sizeof(score[i]));  // Write to the open file
    fout.close();
}

// Getting game results from the file
long FileSystem::load_records(long *score, int map_size, int level)
{
    std::string path_str = getenv("HOME");
    path_str += MAIN_PATH_STR;
    path_str += FILE_NAME_SCORE_STR;
    fs::path file_path(path_str);
    
    std::ifstream fin(file_path.native(), std::ios::binary);
    if (!fin.is_open())
        fs::create_directory(file_path.parent_path().native());
    
    for (unsigned int i = 0; i < 30; ++i)
        fin.read((char*)&score[i], sizeof(score[i]));   // Read all
    fin.close();
    return score[10*map_size + (level - 1)];             // Return the record of the current game
}

// Write settings to the file
void FileSystem::save_settings(CONFIG &conf, int *control)
{
    std::string path_str = getenv("HOME");
    path_str += MAIN_PATH_STR;
    path_str += FILE_NAME_SETTINGS_STR;
    fs::path file_path(path_str);
    
    std::ofstream fout(file_path.native(), std::ios::binary);
    fout.write((char*)&conf, sizeof(conf));                 // Write the bite field to the config file
    for (unsigned int i = 0; i < 4; ++i)
        fout.write((char*)&control[i], sizeof(control[i])); // Write the control settings
    fout.close();
}

// Getting settings from the file
CONFIG FileSystem::load_settings(int *control)
{
    std::string path_str = getenv("HOME");
    path_str += MAIN_PATH_STR;
    path_str += FILE_NAME_SETTINGS_STR;
    fs::path file_path(path_str);
    
    CONFIG conf = {5, 1, 1, 0, 0, 0};
    
    std::ifstream fin(file_path.native(), std::ios::binary);
    if (!fin.is_open()) {
        fs::create_directory(file_path.parent_path().native());
        return conf;
    }
    
    fin.read((char*)&conf, sizeof(conf));
    for (unsigned int i = 0; i < 4; ++i)
        fin.read((char*)&control[i], sizeof(control[i]));
    fin.close();
    return conf;
}
