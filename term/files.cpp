#include <experimental/filesystem>
#include <fstream>
#include <string>
#include "files.h"

namespace fs = std::experimental::filesystem;

// Writing the game result to the file
void FileSystem::saveRecords(long *score)
{
	std::string pathStr = getenv("HOME");
	pathStr += MAIN_PATH_STR;
	pathStr += FILE_NAME_SCORE_STR;
	fs::path filePath(pathStr);
	
	std::ofstream fout(filePath.native(), std::ios::binary);
	for(unsigned int i = 0; i < 30; i++)                // 3 map types * 10 speed level = 30 
		fout.write((char*)&score[i], sizeof(score[i])); // Write to the open file
	fout.close();
}

// Getting game results from the file
long FileSystem::loadRecords(long *score, int MapSize, int level)
{
	std::string pathStr = getenv("HOME");
	pathStr += MAIN_PATH_STR;
	pathStr += FILE_NAME_SCORE_STR;
	fs::path filePath(pathStr);
	
	std::ifstream fin(filePath.native(), std::ios::binary);
	if(!fin)
		fs::create_directory(filePath.parent_path().native());
	for(unsigned int i = 0; i < 30; i++)
		fin.read((char*)&score[i], sizeof(score[i]));   // Read all
	fin.close();
	return score[10*MapSize + (level - 1)];            // Return the record of the current game
}

// Write settings to the file
void FileSystem::saveSettings(CONFIG &conf, int *control)
{
	std::string pathStr = getenv("HOME");
	pathStr += MAIN_PATH_STR;
	pathStr += FILE_NAME_SETTINGS_STR;
	fs::path filePath(pathStr);
	
	std::ofstream fout(filePath.native(), std::ios::binary);
	fout.write((char*)&conf, sizeof(conf));                 // Write the bite field to the config file
	for(unsigned int i = 0; i < 4; i++)
		fout.write((char*)&control[i], sizeof(control[i])); // Write the control settings
	fout.close();
}

// Getting settings from the file
CONFIG FileSystem::loadSettings(int *control)
{
	std::string pathStr = getenv("HOME");
	pathStr += MAIN_PATH_STR;
	pathStr += FILE_NAME_SETTINGS_STR;
	fs::path filePath(pathStr);
	
	std::ifstream fin(filePath.native(), std::ios::binary);
	if(!fin)
		fs::create_directory(filePath.parent_path().native());
	
	CONFIG conf;
	fin.read((char*)&conf, sizeof(conf));
	for(unsigned int i = 0; i < 4; i++)
		fin.read((char*)&control[i], sizeof(control[i]));
	fin.close();
	return conf;
}
