// System Libraries
#include <iostream>

// User libraries
#include "Backend.h"

Backend::~Backend(){
    closeDatabase();
}

void Backend::initialize(){
    db = nullptr; 
    bool success = openDatabase("data/OpenMusic.db");
    if (success){
        std::cout << "[Database] Sucessfully initialized.\n";
    }else{
        std::cout << "[Database] Could not be created.\n";
    }
}

void Backend::closeDatabase(){
    if (db){
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Backend::openDatabase(const std::string& dbName){
    if (db){
        // TODO: change into a messagebox instead
        std::cout << "[WARNING] The database already open!\n";
        return false;
    }

    // Converts dbName into a character array and references the DB variable
    int statusCode = sqlite3_open(dbName.c_str(), &db);
    if (statusCode != SQLITE_OK){
        std::cout << "[WARNING] The database cannot be created! Error message: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    
    return true;
}

bool Backend::executeSQL(const std::string& sql){
    char errorMessage;
    int statusCode = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);
    
    if (statusCode != SQLITE_OK){
        std::cout << "[WARNING] SQL execution failed! Error message: " << errorMessage << "\n";
        sqlite3_free(errorMessage);
        return false;
    }
    
    return true;
}