// System Libraries
#include <iostream>

// User libraries
#include "Backend.h"

Backend::Backend(){
    // Initializing the database to a null value in case it has anything garbage
    db = nullptr; 
    bool success = openDatabase("OpenMusic.db");
    if (success){
        std::cout << "[Database] Sucessfully initialized.\n";
    }else{
        std::cout << "[Database] Could not be created.\n";
    }
}

Backend::~Backend(){
    closeDatabase();
}

void Backend::closeDatabase(){

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