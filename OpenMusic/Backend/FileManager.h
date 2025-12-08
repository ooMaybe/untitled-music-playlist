/*****************************
    Author: Karam Zako
    Description: ...

******************************/

// Checks if BACKEND is not defined & prevents the file from being incldued multiple times
#ifndef FILEMANAGER_H
// Defines the file so that future "include" statements are skipped
#define FILEMANAGER_H

#include <string>

// Creates a new class (blueprint) of Backend
class FileManager {
    
public:
    bool createFolder(const std::string& path);
    bool createFile(const std::string& path);

    bool deleteFolder(const std::string& path);
    bool deleteFile(const std::string& path);

    bool exists(const std::string& path);
};

#endif
