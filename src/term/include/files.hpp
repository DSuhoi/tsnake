/*** * * * * * * * * * * * * * * * * * * * * ***
 * files.h                                      *
 * The library for working with the file system *
 * (writing and reading data from files).       *
 *                                              *
 * Created by DSuhoi (2020) (C++17)             *
 *** * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __FILES_HPP__
#define __FILES_HPP__


#include <filesystem>
#include <fstream>
#include <string>


constexpr const char MAIN_PATH_STR [] = "/.local/share/tsnake/";  // The path to the preferences directory and maps
constexpr const char FILE_NAME_SCORE_STR [] = "score.tsn";        // Path to the file with the table of records
constexpr const char FILE_NAME_SETTINGS_STR [] = "settings.tsn";  // Path to the settings file

// Bit field settings of the card
struct CONFIG
{   
    unsigned speed:       4;   // Speed of the player [1..10]
    unsigned map_size:    2;   // Map size [0..2]
    unsigned num_fruits:  7;   // Number of fruits [1..99]
    unsigned border:      1;   // Borders 1/0
    unsigned teleport:    1;   // Snake teleportation 1/0
    unsigned clear_score: 1;   // Clearing records 1/0
};

// File System class
struct FileSystem
{
    FileSystem() = delete;
    static void save_records(long *score);                            // Writing the game result to the file
    static void save_settings(CONFIG &conf, int *control);            // Write settings to the file
    static long load_records(long *score, int map_size, int level);   // Getting game results from the file
    static CONFIG load_settings(int *control);                        // Getting settings from the file
};

#endif
