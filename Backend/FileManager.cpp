#include <filesystem>
#include <fstream>
#include <iostream>

#include <FileManager.h>

using namespace std;

bool createFolder(const string& path){
    // Try statement is used so that if ther eare errors, the program wont crash
    try{
        if (filesystem::create_directories(path)){
            cout << "[FileManager] Successfully created the folder: " << path << "!\n";
            return true;
        }else{
            cout << "[FileManager] Failed to create the folder: " << path << "! Folder either already exists or could not be created.\n";
            return false;
        }
    }catch(const filesystem::filesystem_error& ex){
        cout << "[FileManager] An error occured while creating the folder: " << path << ". Error message: " << ex.what() << ".\n";
        return false;
    }
}

bool createFile(const string& path){
    // Creates a new stream variable
    fstream file(path, ios::out);

    // Try statement is used so that if ther eare errors, the program wont crash
    try{
        if (file.is_open()){
            file.close();
            cout << "[FileManager] Successfully created the file: " << path << "!\n";
            return true;
        }else{
            cout << "[FileManager] Failed to create the file: " << path << "! File either already exists or could not be created.\n";
            return false;
        }
    }catch(const exception& ex){
        cout << "[FileManager] An error occured while creating the file: " << path << ". Error message: " << ex.what() << ".\n";
        return false;
    }
    return true;
}

bool deleteFolder(const string& path);
bool deleteFile(const string& path);

bool exists(const string& path);