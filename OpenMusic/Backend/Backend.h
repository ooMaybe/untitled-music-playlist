/*****************************
    Author: Karam Zako
    Description: ...

******************************/

// Checks if BACKEND is not defined & prevents the file from being incldued multiple times
#ifndef BACKEND_H
// Defines the file so that future "include" statements are skipped
#define BACKEND_H

#include <string>

// Since the SQLLite library is a C library, we have to port it into C++
// This is done using the "extern" keyword and then importing the C library
extern "C" {
    #include "../APIs/SQLlite/sqlite3.h"
}

// Creates a new class (blueprint) of Backend
class Backend {
private:
    // A variable which will store the connect of the database
    sqlite3* db;
    
public:
    // Deconstructer, code in here will be executed when the class is being deleted
    ~Backend();
    
    void initialize();
    bool openDatabase(const std::string& dbName);
    void closeDatabase();
    bool executeSQL(const std::string& sql);
};

#endif
