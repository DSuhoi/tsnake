#include <experimental/filesystem>
#include <fstream>
#include <cstring>
#include "files.h"

namespace fs = std::experimental::filesystem;

// Writing the game result to the file
void FileSystem::SaveRecords(long *score)
{
	std::ofstream fout(FILE_NAME_SCORE_STR, std::ios::binary);
	for(int i = 0; i < 30; i++){	// 3 map types * 10 speed level = 30 
		fout.write((char*)&score[i], sizeof(score[i]));	// Write to the open file
	}
	fout.close();
}

// Getting game results from the file
long FileSystem::LoadRecords(long *score, int MapSize, int level)
{
	std::ifstream fin(FILE_NAME_SCORE_STR, std::ios::binary);
	for(int i = 0; i < 30; i++){
		fin.read((char*)&score[i], sizeof(score[i]));	// Read all
	}
	fin.close();
	return score[10*MapSize + (level - 1)];	// Return the record of the current game
}

// Write settings to the file
void FileSystem::SaveSettings(CONFIG &conf, int *control)
{
	std::ofstream fout(FILE_NAME_SETTINGS_STR, std::ios::binary);
	fout.write((char*)&conf, sizeof(conf));	// Write the bite field to the config file
	for(int i = 0; i < 4; i++){
		fout.write((char*)&control[i], sizeof(control[i]));	// Write the control settings
	}
	fout.close();
}

// Getting settings from the file
CONFIG FileSystem::LoadSettings(int *control)
{
	CONFIG conf;
	std::ifstream fin(FILE_NAME_SETTINGS_STR, std::ios::binary);
	fin.read((char*)&conf, sizeof(conf));
	for(int i = 0; i < 4; i++){
		fin.read((char*)&control[i], sizeof(control[i]));
	}
	fin.close();
	return conf;
}
