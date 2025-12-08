/*****************************************************************************\
    Class: Backend.h
    Description: Header file for Backend class which manages SQLite database
        operations for playlist management, song storage, and downloaded media tracking.
\*****************************************************************************/

// Checks if BACKEND is not defined & prevents the file from being included multiple times
#ifndef BACKEND_H
// Defines the file so that future "include" statements are skipped
#define BACKEND_H

#include <string>
#include <iostream>
#include <vector>

/**********************************\
    EXTERNAL C LIBRARY IMPORT
\**********************************/

// Since the SQLite library is a C library, we have to port it into C++
// This is done using the "extern" keyword and then importing the C library
extern "C" {
    #include "../APIs/SQLlite/sqlite3.h"
}

/**********************************\
    DATA STRUCTURES
\**********************************/

/*****************************************************************************\
    Struct: PlaylistSong
    Description: Stores metadata for a song within a playlist
\*****************************************************************************/
struct PlaylistSong {
    int id;                         // Unique identifier for the song in database
    std::string title;              // Title of the song
    std::string artist;             // Artist name
    std::string duration;           // Song duration (e.g., "3:45")
    std::string dateAdded;          // Date when song was added to playlist
    std::string filePath;           // Path to the audio file
    std::string thumbnailPath;      // Path to the song's thumbnail image
};

/*****************************************************************************\
    Struct: Playlist
    Description: Stores basic information about a playlist
\*****************************************************************************/
struct Playlist {
    std::string name;               // Name of the playlist (primary key)
    std::string imagePath;          // Path to the playlist cover image
};

/*****************************************************************************\
    Struct: DownloadedSong
    Description: Stores metadata for downloaded songs not in any playlist
\*****************************************************************************/
struct DownloadedSong {
    std::string title;              // Title of the downloaded song
    std::string artist;             // Artist name
    std::string duration;           // Song duration (e.g., "3:45")
    std::string dateAdded;          // Date when song was downloaded
    std::string filePath;           // Path to the audio file (primary key)
    std::string thumbnailPath;      // Path to the song's thumbnail image
};

/**********************************\
    BACKEND CLASS DEFINITION
\**********************************/

/*****************************************************************************\
    Class: Backend
    Description: Manages all database operations for the OpenMusic application
        including playlist management, song storage, and query execution.
\*****************************************************************************/
class Backend {
private:
    /**********************************\
        PRIVATE MEMBERS
    \**********************************/
    
    sqlite3* db;                    // Pointer to SQLite database connection
    
    /*****************************************************************************\
        Function: selectCallback
        Description: Static callback function for processing SELECT query results
        Parameters: 
            void *data -> pointer to data structure storing results
            int argc -> number of columns in result
            char **argv -> array of column values
            char **colName -> array of column names
        Returns: 
            int -> 0 to continue processing rows
    \*****************************************************************************/
    static int selectCallback(void *data, int argc, char **argv, char **colName);
    
public:
    /**********************************\
        PUBLIC MEMBERS
    \**********************************/
    
    /*****************************************************************************\
        Function: ~Backend (Destructor)
        Description: Cleans up resources when Backend object is destroyed
        Parameters: nothing
        Returns: nothing
    \*****************************************************************************/
    ~Backend();
    
    /**********************************\
        DATABASE OPERATIONS
    \**********************************/
    
    /*****************************************************************************\
        Function: initialize
        Description: Initializes database connection and creates tables
        Parameters: 
            std::string &dbPath -> path to the database file
        Returns: nothing
    \*****************************************************************************/
    void initialize(std::string &dbPath);
    
    /*****************************************************************************\
        Function: openDatabase
        Description: Opens a connection to the SQLite database
        Parameters: 
            const std::string& dbName -> name/path of database file
        Returns: 
            bool -> true if successful, false otherwise
    \*****************************************************************************/
    bool openDatabase(const std::string& dbName);
    
    /*****************************************************************************\
        Function: closeDatabase
        Description: Closes the database connection safely
        Parameters: nothing
        Returns: nothing
    \*****************************************************************************/
    void closeDatabase();
    
    /*****************************************************************************\
        Function: executeSQL
        Description: Executes a SQL statement on the database
        Parameters: 
            const std::string& sql -> SQL query string to execute
        Returns: 
            bool -> true if execution successful, false otherwise
    \*****************************************************************************/
    bool executeSQL(const std::string& sql);
    
    /**********************************\
        PLAYLIST OPERATIONS
    \**********************************/
    
    /*****************************************************************************\
        Function: createTables
        Description: Creates all necessary database tables if they don't exist
        Parameters: nothing
        Returns: 
            bool -> true if all tables created successfully, false otherwise
    \*****************************************************************************/
    bool createTables();
    
    /*****************************************************************************\
        Function: savePlaylist
        Description: Saves or updates a playlist in the database
        Parameters: 
            const std::string& name -> playlist name (primary key)
            const std::string& imagePath -> path to playlist cover image
        Returns: 
            bool -> true if save successful, false otherwise
    \*****************************************************************************/
    bool savePlaylist(const std::string& name, const std::string& imagePath);
    
    /*****************************************************************************\
        Function: addSongToPlaylist
        Description: Adds a song to a specific playlist
        Parameters: 
            const std::string& playlistName -> name of the playlist
            const std::string& title -> song title
            const std::string& artist -> artist name
            const std::string& duration -> song duration
            const std::string& dateAdded -> date song was added
            const std::string& filePath -> path to audio file
            const std::string& thumbnailPath -> path to thumbnail image
        Returns: 
            bool -> true if song added successfully, false otherwise
    \*****************************************************************************/
    bool addSongToPlaylist(const std::string& playlistName, const std::string& title,
                           const std::string& artist, const std::string& duration,
                           const std::string& dateAdded, const std::string& filePath,
                           const std::string& thumbnailPath);

    /*****************************************************************************\
        Function: getPlaylistSongs
        Description: Retrieves all songs from a specific playlist
        Parameters: 
            const std::string& playlistName -> name of the playlist to query
        Returns: 
            std::vector<PlaylistSong> -> vector containing all songs in playlist
    \*****************************************************************************/
    std::vector<PlaylistSong> getPlaylistSongs(const std::string& playlistName);
    
    /*****************************************************************************\
        Function: getAllPlaylists
        Description: Retrieves all playlists from the database
        Parameters: nothing
        Returns: 
            std::vector<Playlist> -> vector containing all playlists
    \*****************************************************************************/
    std::vector<Playlist> getAllPlaylists();
    
    /*****************************************************************************\
        Function: removeSongFromPlaylist
        Description: Removes a specific song from a playlist
        Parameters: 
            const std::string& playlistName -> name of the playlist
            int songId -> ID of the song to remove
        Returns: 
            bool -> true if removal successful, false otherwise
    \*****************************************************************************/
    bool removeSongFromPlaylist(const std::string& playlistName, int songId);
    
    /*****************************************************************************\
        Function: deletePlaylist
        Description: Deletes a playlist and all its songs from the database
        Parameters: 
            const std::string& playlistName -> name of the playlist to delete
        Returns: 
            bool -> true if deletion successful, false otherwise
    \*****************************************************************************/
    bool deletePlaylist(const std::string& playlistName);
    
    /*****************************************************************************\
        Function: getPlaylistImage
        Description: Retrieves the image path for a specific playlist
        Parameters: 
            const std::string& playlistName -> name of the playlist
        Returns: 
            std::string -> path to the playlist's image
    \*****************************************************************************/
    std::string getPlaylistImage(const std::string& playlistName);
    
    /**********************************\
        DOWNLOADED SONGS OPERATIONS
    \**********************************/
    
    /*****************************************************************************\
        Function: saveDownloadedSong
        Description: Saves a downloaded song's metadata to the database
        Parameters: 
            const std::string& title -> song title
            const std::string& artist -> artist name
            const std::string& duration -> song duration
            const std::string& dateAdded -> date song was downloaded
            const std::string& filePath -> path to audio file (primary key)
            const std::string& thumbnailPath -> path to thumbnail image
        Returns: 
            bool -> true if save successful, false otherwise
    \*****************************************************************************/
    bool saveDownloadedSong(const std::string& title, const std::string& artist,
                           const std::string& duration, const std::string& dateAdded,
                           const std::string& filePath, const std::string& thumbnailPath);
    
    /*****************************************************************************\
        Function: getAllDownloadedSongs
        Description: Retrieves all downloaded songs from the database
        Parameters: nothing
        Returns: 
            std::vector<DownloadedSong> -> vector containing all downloaded songs
    \*****************************************************************************/
    std::vector<DownloadedSong> getAllDownloadedSongs();
    
    /*****************************************************************************\
        Function: removeDownloadedSong
        Description: Removes a downloaded song from the database
        Parameters: 
            const std::string& filePath -> path of the song file to remove
        Returns: 
            bool -> true if removal successful, false otherwise
    \*****************************************************************************/
    bool removeDownloadedSong(const std::string& filePath);
};

#endif
