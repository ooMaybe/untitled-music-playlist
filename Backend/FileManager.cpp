#include <filesystem>
#include <fstream>
#include <iostream>

#include "FileManager.h"

using namespace std;

bool FileManager::createFolder(const string& path){
    // Try statement is used so that if ther eare errors, the program wont crash
    try{
        if (filesystem::create_directories(path)){
            cout << "[FileManager] Successfully created the folder \"" << path << "\"!\n";
            return true;
        }else{
            if (filesystem::exists(path)){
                cout << "[FileManager] The folder \"" << path << "\" already exists!\n";
                return false;
            } else{
                cout << "[FileManager] The folder \"" << path << "\" could not be created for an unknown reason!\n";
                return false;
            }
        }
    }catch(const filesystem::filesystem_error& ex){
        cout << "[FileManager] An error occured while creating the folder \"" << path << "\". Error message: " << ex.what() << ".\n";
        return false;
    }
}

bool FileManager::createFile(const string& path){
    // Creates a new stream variable
    fstream file(path, ios::out);

    // Try statement is used so that if ther eare errors, the program wont crash
    try{
        if (file.is_open()){
            file.close();
            cout << "[FileManager] Successfully created the file \"" << path << "\"!\n";
            return true;
        }else{
            if (filesystem::exists(path)){
                cout << "[FileManager] The file \"" << path << "\" already exists!\n";
                return false;
            }else{
                cout << "[FileManager] The file \"" << path << "\" could not be created for an unknown reason!\n";
                return false;
            }
        }
    }catch(const exception& ex){
        cout << "[FileManager] An error occured while creating the file \"" << path << "\". Error message: " << ex.what() << ".\n";
        return false;
    }
    return true;
}

bool FileManager::deleteFolder(const string& path){
    if (exists(path)){
        try{
            filesystem::remove_all(path);
            cout << "[FileManager] Successfully deleted the folder \"" << path << "\"!\n";
            return true;
        }catch(const filesystem::filesystem_error& ex){
            cout << "[FileManager] An error occured while deleting the folder \"" << path << "\". Error message: " << ex.what() << ".\n";
            return false;
        }
    }else{
        cout << "[FileManager] The folder \"" << path << "\" does not exist!\n";
        return false;
    }
}

bool FileManager::deleteFile(const string& path){
    if (exists(path)){
        try{
            filesystem::remove(path);
            cout << "[FileManager] Successfully deleted the file \"" << path << "\"!\n";
            return true;
        }catch(const filesystem::filesystem_error& ex){
            cout << "[FileManager] An error occured while deleting the file \"" << path << "\". Error message: " << ex.what() << ".\n";
            return false;
        }
    }else{
        cout << "[FileManager] The file \"" << path << "\" does not exist!\n";
        return false;
    }
}

bool FileManager::exists(const string& path){
    return filesystem::exists(path);
}