/*** * * * * * * * * * * * * * * * * * * * * ***
 * files.h                                      *
 * The library for working with the file system *
 * (writing and reading data from files).       *
 *                                             *
 * Created by DSuhoi (2020) (C++11)            *
 *** * * * * * * * * * * * * * * * * * * * * ***/


#ifndef __FILES_H__
#define __FILES_H__

const char MAIN_PATH_STR [] = "/.local/share/tsnake/";  // The path to the preferences directory and maps
const char FILE_NAME_SCORE_STR [] = "score.tsn";        // Path to the file with the table of records
const char FILE_NAME_SETTINGS_STR [] = "settings.tsn";  // Path to the settings file

struct CONFIG {         // Bit field settings of the card
unsigned speed: 4;      // Speed of the player [1..10]
unsigned mapSize: 2;    // Map size [0..2]
unsigned numFruits: 7;  // Number of fruits [1..99]
unsigned border: 1;     // Borders 1/0
unsigned teleport: 1;   // Snake teleportation 1/0
unsigned clearScore: 1; // Clearing records 1/0
};

// File System class
class FileSystem{
private:
    FileSystem(){}
public:
    static void saveRecords(long *score);                           // Writing the game result to the file
    static void saveSettings(CONFIG &conf, int *control);           // Write settings to the file
    static long loadRecords(long *score, int MapSize, int level);   // Getting game results from the file
    static CONFIG loadSettings(int *control);                       // Getting settings from the file
};

#endif
