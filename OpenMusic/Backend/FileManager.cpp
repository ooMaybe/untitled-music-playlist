/*****************************************************************************\
    Class: FileManager.cpp
    Description: FileManager class implementation for handling file system operations
        including creating, deleting, and checking existence of files and folders.
\*****************************************************************************/

#include <filesystem>
#include <fstream>
#include <iostream>

#include "FileManager.h"

using namespace std;

/*****************************************************************************\
    Function: createFolder
    Description: Creates a new folder at the specified path
    Parameters: 
        const string& path -> path where the folder should be created
    Returns: 
        bool -> true if folder created successfully, false otherwise
\*****************************************************************************/
bool FileManager::createFolder(const string& path){
    /**********************************\
        FOLDER CREATION WITH ERROR HANDLING
    \**********************************/
    
    // Try statement is used so that if there are errors, the program won't crash
    try{
        // Attempts to create directories including parent directories if needed
        if (filesystem::create_directories(path)){
            cout << "[FileManager] Successfully created the folder \"" << path << "\"!\n";
            return true;
        }else{
            // Checks if folder already exists
            if (filesystem::exists(path)){
                cout << "[FileManager] The folder \"" << path << "\" already exists!\n";
                return false;
            } else{
                cout << "[FileManager] The folder \"" << path << "\" could not be created for an unknown reason!\n";
                return false;
            }
        }
    }catch(const filesystem::filesystem_error& ex){
        // Catches filesystem errors and displays error message
        cout << "[FileManager] An error occured while creating the folder \"" << path << "\". Error message: " << ex.what() << ".\n";
        return false;
    }
}

/*****************************************************************************\
    Function: createFile
    Description: Creates a new file at the specified path
    Parameters: 
        const string& path -> path where the file should be created
    Returns: 
        bool -> true if file created successfully, false otherwise
\*****************************************************************************/
bool FileManager::createFile(const string& path){
    /**********************************\
        FILE CREATION
    \**********************************/
    
    // Creates a new file stream variable in output mode
    fstream file(path, ios::out);

    /**********************************\
        FILE CREATION ERROR HANDLING
    \**********************************/
    
    // Try statement is used so that if there are errors, the program won't crash
    try{
        // Checks if the file stream opened successfully
        if (file.is_open()){
            // Closes the file stream after creation
            file.close();
            cout << "[FileManager] Successfully created the file \"" << path << "\"!\n";
            return true;
        }else{
            // Checks if file already exists
            if (filesystem::exists(path)){
                cout << "[FileManager] The file \"" << path << "\" already exists!\n";
                return false;
            }else{
                cout << "[FileManager] The file \"" << path << "\" could not be created for an unknown reason!\n";
                return false;
            }
        }
    }catch(const exception& ex){
        // Catches general exceptions and displays error message
        cout << "[FileManager] An error occured while creating the file \"" << path << "\". Error message: " << ex.what() << ".\n";
        return false;
    }
    
    // Returns true as fallback (though unreachable due to try-catch)
    return true;
}

/*****************************************************************************\
    Function: deleteFolder
    Description: Deletes a folder and all its contents at the specified path
    Parameters: 
        const string& path -> path of the folder to delete
    Returns: 
        bool -> true if folder deleted successfully, false otherwise
\*****************************************************************************/
bool FileManager::deleteFolder(const string& path){
    /**********************************\
        FOLDER EXISTENCE CHECK
    \**********************************/
    
    // Verifies that the folder exists before attempting deletion
    if (exists(path)){
        /**********************************\
            FOLDER DELETION
        \**********************************/
        
        try{
            // Recursively removes folder and all contents
            filesystem::remove_all(path);
            cout << "[FileManager] Successfully deleted the folder \"" << path << "\"!\n";
            return true;
        }catch(const filesystem::filesystem_error& ex){
            // Catches filesystem errors during deletion
            cout << "[FileManager] An error occured while deleting the folder \"" << path << "\". Error message: " << ex.what() << ".\n";
            return false;
        }
    }else{
        // Folder doesn't exist, cannot delete
        cout << "[FileManager] The folder \"" << path << "\" does not exist!\n";
        return false;
    }
}

/*****************************************************************************\
    Function: deleteFile
    Description: Deletes a file at the specified path
    Parameters: 
        const string& path -> path of the file to delete
    Returns: 
        bool -> true if file deleted successfully, false otherwise
\*****************************************************************************/
bool FileManager::deleteFile(const string& path){
    /**********************************\
        FILE EXISTENCE CHECK
    \**********************************/
    
    // Verifies that the file exists before attempting deletion
    if (exists(path)){
        /**********************************\
            FILE DELETION
        \**********************************/
        
        try{
            // Removes the specified file
            filesystem::remove(path);
            cout << "[FileManager] Successfully deleted the file \"" << path << "\"!\n";
            return true;
        }catch(const filesystem::filesystem_error& ex){
            // Catches filesystem errors during deletion
            cout << "[FileManager] An error occured while deleting the file \"" << path << "\". Error message: " << ex.what() << ".\n";
            return false;
        }
    }else{
        // File doesn't exist, cannot delete
        cout << "[FileManager] The file \"" << path << "\" does not exist!\n";
        return false;
    }
}

/*****************************************************************************\
    Function: exists
    Description: Checks if a file or folder exists at the specified path
    Parameters: 
        const string& path -> path to check for existence
    Returns: 
        bool -> true if path exists, false otherwise
\*****************************************************************************/
bool FileManager::exists(const string& path){
    // Uses filesystem library to check if path exists
    return filesystem::exists(path);
}