/*****************************
    Author: Karam Zako
    Description: ...

******************************/

// Checks if BACKEND is not defined & prevents the file from being incldued multiple times
#ifndef BACKEND_H
// Defines the file so that future "include" statements are skipped
#define BACKEND_H

#include <string>
#include <iostream>
#include <vector>

// Since the SQLLite library is a C library, we have to port it into C++
// This is done using the "extern" keyword and then importing the C library
extern "C" {
    #include "../APIs/SQLlite/sqlite3.h"
}

// Creates a new class (blueprint) of Backend
struct PlaylistSong {
    int id;
    std::string title;
    std::string artist;
    std::string duration;
    std::string dateAdded;
    std::string filePath;
    std::string thumbnailPath;
};

struct Playlist {
    std::string name;
    std::string imagePath;
};

struct DownloadedSong {
    std::string title;
    std::string artist;
    std::string duration;
    std::string dateAdded;
    std::string filePath;
    std::string thumbnailPath;
};

class Backend {
private:
    // A variable which will store the connect of the database
    sqlite3* db;
    
    static int selectCallback(void *data, int argc, char **argv, char **colName);
    
public:
    // Deconstructer, code in here will be executed when the class is being deleted
    ~Backend();
    
    void initialize(std::string &dbPath);
    bool openDatabase(const std::string& dbName);
    void closeDatabase();
    bool executeSQL(const std::string& sql);
    
    // Playlist operations
    bool createTables();
    bool savePlaylist(const std::string& name, const std::string& imagePath);
    bool addSongToPlaylist(const std::string& playlistName, const std::string& title,
                           const std::string& artist, const std::string& duration,
                           const std::string& dateAdded, const std::string& filePath,
                           const std::string& thumbnailPath);

    std::vector<PlaylistSong> getPlaylistSongs(const std::string& playlistName);
    std::vector<Playlist> getAllPlaylists();
    bool removeSongFromPlaylist(const std::string& playlistName, int songId);
    bool deletePlaylist(const std::string& playlistName);
    std::string getPlaylistImage(const std::string& playlistName);
    
    // Downloaded songs operations
    bool saveDownloadedSong(const std::string& title, const std::string& artist,
                           const std::string& duration, const std::string& dateAdded,
                           const std::string& filePath, const std::string& thumbnailPath);
    std::vector<DownloadedSong> getAllDownloadedSongs();
    bool removeDownloadedSong(const std::string& filePath);
};

#endif
