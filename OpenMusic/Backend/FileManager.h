/*****************************
    Description: The header class for file and folder management operations
******************************/

// Checks if BACKEND is not defined & prevents the file from being incldued multiple times
#ifndef FILEMANAGER_H
// Defines the file so that future "include" statements are skipped
#define FILEMANAGER_H

/**********************************\
    STANDARD LIBRARY INCLUDES
\**********************************/
#include <string>

// Creates a new class (blueprint) of Backend
class FileManager {
    
public:
    /**********************************\
        FILEMANAGER OPERATIONS
    \**********************************/

    /*****************************************************************************\
        Function: createFolder
        Description: Creates a new folder at the specified path
        Parameters: 
            const std::string& path -> path where the folder will be created
        Returns: 
            bool -> true if creation successful, false otherwise
    \*****************************************************************************/
    bool createFolder(const std::string& path);

    /*****************************************************************************\
        Function: createFile
        Description: Creates a new file at the specified path
        Parameters: 
            const std::string& path -> path where the file will be created
        Returns: 
            bool -> true if creation successful, false otherwise
    \*****************************************************************************/
    bool createFile(const std::string& path);

    /*****************************************************************************\
        Function: deleteFolder
        Description: Deletes a folder at the specified path
        Parameters: 
            const std::string& path -> path of the folder to delete
        Returns: 
            bool -> true if deletion successful, false otherwise
    \*****************************************************************************/
    bool deleteFolder(const std::string& path);

    /*****************************************************************************\
        Function: deleteFile
        Description: Deletes a file at the specified path
        Parameters: 
            const std::string& path -> path of the file to delete
        Returns: 
            bool -> true if deletion successful, false otherwise
    \*****************************************************************************/
    bool deleteFile(const std::string& path);

    /*****************************************************************************\
        Function: exists
        Description: Checks if a file or folder exists at the specified path
        Parameters: 
            const std::string& path -> path to check for existence
        Returns: 
            bool -> true if path exists, false otherwise
    \*****************************************************************************/
    bool exists(const std::string& path);
};

#endif
