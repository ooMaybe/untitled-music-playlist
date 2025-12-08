/*****************************************************************************\
    Class: Backend.cpp
    Description: Backend class implementation for managing SQLite database operations
        including playlist management, song storage, and database initialization.
\*****************************************************************************/

// Our own libraries
#include "Backend.h"

// Other libraries
#include <QDebug>

/*****************************************************************************\
    Function: ~Backend (Destructor)
    Description: Cleans up resources by closing the database connection
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
Backend::~Backend(){
    closeDatabase();
}

/*****************************************************************************\
    Function: initialize
    Description: Initializes the database connection and creates necessary tables
    Parameters: 
        std::string &dbPath -> path to the database file
    Returns: nothing
\*****************************************************************************/
void Backend::initialize(std::string &dbPath){
    /**********************************\
        DATABASE INITIALIZATION
    \**********************************/
    
    // Sets database pointer to null before initialization
    db = nullptr;
    
    // Attempts to open the database at specified path
    bool success = openDatabase(dbPath);
    
    if (success){
        qDebug() << "[Database] Sucessfully initialized.\n";
        
        // Creates required tables if database opened successfully
        createTables();
    }else{
        qDebug() << "[Database] Could not be created.\n";
    }
}

/*****************************************************************************\
    Function: closeDatabase
    Description: Safely closes the database connection and resets the pointer
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void Backend::closeDatabase(){
    // Checks if database connection exists before closing
    if (db){
        // Closes the SQLite database connection
        sqlite3_close(db);
        
        // Resets database pointer to null
        db = nullptr;
    }
}

/*****************************************************************************\
    Function: openDatabase
    Description: Opens a connection to the SQLite database
    Parameters: 
        const std::string& dbName -> name/path of the database file
    Returns: 
        bool -> true if successful, false otherwise
\*****************************************************************************/
bool Backend::openDatabase(const std::string& dbName){
    /**********************************\
        DATABASE CONNECTION CHECK
    \**********************************/
    
    // Prevents opening multiple database connections
    if (db){
        // TODO: change into a messagebox instead
        std::cout << "[WARNING] The database already open!\n";
        return false;
    }

    /**********************************\
        OPEN DATABASE CONNECTION
    \**********************************/
    
    // Converts dbName into a character array and references the DB variable
    int statusCode = sqlite3_open(dbName.c_str(), &db);
    
    // Checks if database opened successfully
    if (statusCode != SQLITE_OK){
        std::cout << "[WARNING] The database cannot be created! Error message: " << sqlite3_errmsg(db) << "\n";
        return false;
    }
    
    // Returns true if database connection established successfully
    return true;
}

/*****************************************************************************\
    Function: executeSQL
    Description: Executes a SQL statement on the database
    Parameters: 
        const std::string& sql -> SQL query string to execute
    Returns: 
        bool -> true if execution successful, false otherwise
\*****************************************************************************/
bool Backend::executeSQL(const std::string& sql){
    // Initializes error message pointer
    char* errorMessage = nullptr;
    
    // Executes the SQL statement on the database
    int statusCode = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);    
    
    // Checks if SQL execution failed
    if (statusCode != SQLITE_OK){
        std::cout << "[WARNING] SQL execution failed! Error message: " << errorMessage << "\n";
        
        // Frees the error message memory allocated by SQLite
        sqlite3_free(errorMessage);
        return false;   
    }
    
    // Returns true if SQL executed successfully
    return true;
}

/*****************************************************************************\
    Function: selectCallback
    Description: Callback function for processing SELECT query results
    Parameters: 
        void *data -> pointer to vector storing results
        int argc -> number of columns in result
        char **argv -> array of column values
        char **colName -> array of column names
    Returns: 
        int -> 0 to continue processing rows
\*****************************************************************************/
int Backend::selectCallback(void *data, int argc, char **argv, char **colName) {
    // Casts void pointer to vector of PlaylistSong objects
    auto *results = static_cast<std::vector<PlaylistSong>*>(data);
    
    // Creates a new PlaylistSong object to store current row data
    PlaylistSong song;
    
    /**********************************\
        PROCESS COLUMN DATA
    \**********************************/
    
    // Iterates through all columns in the result row
    for (int i = 0; i < argc; i++) {
        // Gets column name and value (empty string if NULL)
        std::string column = colName[i];
        std::string value = argv[i] ? argv[i] : "";
        
        // Maps database columns to song object properties
        if (column == "id") song.id = std::stoi(value);
        else if (column == "title") song.title = value;
        else if (column == "artist") song.artist = value;
        else if (column == "duration") song.duration = value;
        else if (column == "date_added") song.dateAdded = value;
        else if (column == "file_path") song.filePath = value;
        else if (column == "thumbnail_path") song.thumbnailPath = value;
    }
    
    // Adds completed song object to results vector
    results->push_back(song);
    
    // Returns 0 to continue processing additional rows
    return 0;
}

/*****************************************************************************\
    Function: createTables
    Description: Creates all necessary database tables if they don't exist
    Parameters: nothing
    Returns: 
        bool -> true if all tables created successfully, false otherwise
\*****************************************************************************/
bool Backend::createTables() {
    /**********************************\
        TABLE CREATION SQL STATEMENTS
    \**********************************/
    
    // SQL to create playlists table with name as primary key
    std::string createPlaylists = 
        "CREATE TABLE IF NOT EXISTS playlists ("
        "name TEXT PRIMARY KEY, "
        "image_path TEXT NOT NULL)";
    
    // SQL to create playlist_songs table with foreign key to playlists
    std::string createSongs = 
        "CREATE TABLE IF NOT EXISTS playlist_songs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "playlist_name TEXT NOT NULL, "
        "title TEXT NOT NULL, "
        "artist TEXT, "
        "duration TEXT, "
        "date_added TEXT, "
        "file_path TEXT, "
        "thumbnail_path TEXT, "
        "FOREIGN KEY(playlist_name) REFERENCES playlists(name) ON DELETE CASCADE)";
    
    // SQL to create downloaded_songs table for tracking downloaded media
    std::string createDownloadedSongs = 
        "CREATE TABLE IF NOT EXISTS downloaded_songs ("
        "file_path TEXT PRIMARY KEY, "
        "title TEXT NOT NULL, "
        "artist TEXT, "
        "duration TEXT, "
        "date_added TEXT, "
        "thumbnail_path TEXT)";
    
    /**********************************\
        EXECUTE TABLE CREATION
    \**********************************/
    
    // Executes all three CREATE TABLE statements and returns true if all succeed
    return executeSQL(createPlaylists) && executeSQL(createSongs) && executeSQL(createDownloadedSongs);
}

/*****************************************************************************\
    Function: savePlaylist
    Description: Saves or updates a playlist in the database
    Parameters: 
        const std::string& name -> playlist name (primary key)
        const std::string& imagePath -> path to playlist cover image
    Returns: 
        bool -> true if save successful, false otherwise
\*****************************************************************************/
bool Backend::savePlaylist(const std::string& name, const std::string& imagePath) {
    // Constructs SQL INSERT statement with OR REPLACE clause for updating existing playlists
    std::string sql = "INSERT OR REPLACE INTO playlists (name, image_path) VALUES ('" 
                      + name + "', '" + imagePath + "')";
    
    // Executes the SQL statement and returns result
    return executeSQL(sql);
}

/*****************************************************************************\
    Function: addSongToPlaylist
    Description: Adds a song to a specific playlist in the database
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
bool Backend::addSongToPlaylist(const std::string& playlistName, const std::string& title,
                                const std::string& artist, const std::string& duration,
                                const std::string& dateAdded, const std::string& filePath,
                                const std::string& thumbnailPath) {
    // Constructs SQL INSERT statement with all song details
    std::string sql = "INSERT INTO playlist_songs (playlist_name, title, artist, duration, date_added, file_path, thumbnail_path) "
                      "VALUES ('" + playlistName + "', '" + title + "', '" + artist + "', '" 
                      + duration + "', '" + dateAdded + "', '" + filePath + "', '" + thumbnailPath + "')";
    
    // Executes the SQL statement and returns result
    return executeSQL(sql);
}

/*****************************************************************************\
    Function: getPlaylistSongs
    Description: Retrieves all songs from a specific playlist
    Parameters: 
        const std::string& playlistName -> name of the playlist to query
    Returns: 
        std::vector<PlaylistSong> -> vector containing all songs in the playlist
\*****************************************************************************/
std::vector<PlaylistSong> Backend::getPlaylistSongs(const std::string& playlistName) {
    // Creates empty vector to store retrieved songs
    std::vector<PlaylistSong> songs;
    
    // Constructs SQL SELECT statement to get all songs from specified playlist
    std::string sql = "SELECT * FROM playlist_songs WHERE playlist_name = '" + playlistName + "'";
    
    /**********************************\
        EXECUTE QUERY
    \**********************************/
    
    // Initializes error message pointer
    char* errorMessage = nullptr;
    
    // Executes query with callback function to populate songs vector
    int statusCode = sqlite3_exec(db, sql.c_str(), selectCallback, &songs, &errorMessage);
    
    // Checks if query execution failed
    if (statusCode != SQLITE_OK) {
        std::cout << "[WARNING] Query failed: " << errorMessage << "\n";
        
        // Frees error message memory
        sqlite3_free(errorMessage);
    }
    
    // Returns vector of songs (empty if query failed)
    return songs;
}

/*****************************************************************************\
    Function: getAllPlaylists
    Description: Retrieves all playlists from the database
    Parameters: nothing
    Returns: 
        std::vector<Playlist> -> vector containing all playlists
\*****************************************************************************/
std::vector<Playlist> Backend::getAllPlaylists() {
    // Creates empty vector to store retrieved playlists
    std::vector<Playlist> playlists;
    
    // Constructs SQL SELECT statement to get all playlists
    std::string sql = "SELECT name, image_path FROM playlists";
    
    /**********************************\
        EXECUTE QUERY WITH LAMBDA
    \**********************************/
    
    // Initializes error message pointer
    char* errorMessage = nullptr;
    
    // Executes query with inline lambda callback to populate playlists vector
    int statusCode = sqlite3_exec(db, sql.c_str(), 
        [](void* data, int argc, char** argv, char** colName) -> int {
            // Casts void pointer to vector of Playlist objects
            auto* playlists = static_cast<std::vector<Playlist>*>(data);
            Playlist p;
            
            // Ensures we have both name and image_path columns
            if (argc >= 2) {
                // Stores playlist name and image path (empty string if NULL)
                p.name = argv[0] ? argv[0] : "";
                p.imagePath = argv[1] ? argv[1] : "";
                playlists->push_back(p);
            }
            return 0;
        }, &playlists, &errorMessage);
    
    // Checks if query execution failed
    if (statusCode != SQLITE_OK) {
        std::cout << "[WARNING] Query failed: " << errorMessage << "\n";
        
        // Frees error message memory
        sqlite3_free(errorMessage);
    }
    
    // Returns vector of playlists (empty if query failed)
    return playlists;
}

/*****************************************************************************\
    Function: removeSongFromPlaylist
    Description: Removes a specific song from a playlist
    Parameters: 
        const std::string& playlistName -> name of the playlist
        int songId -> ID of the song to remove
    Returns: 
        bool -> true if removal successful, false otherwise
\*****************************************************************************/
bool Backend::removeSongFromPlaylist(const std::string& playlistName, int songId) {
    // Constructs SQL DELETE statement to remove song by ID and playlist name
    std::string sql = "DELETE FROM playlist_songs WHERE id = " + std::to_string(songId) 
                      + " AND playlist_name = '" + playlistName + "'";
    
    // Executes the SQL statement and returns result
    return executeSQL(sql);
}

/*****************************************************************************\
    Function: deletePlaylist
    Description: Deletes a playlist and all its songs from the database
    Parameters: 
        const std::string& playlistName -> name of the playlist to delete
    Returns: 
        bool -> true if deletion successful, false otherwise
\*****************************************************************************/
bool Backend::deletePlaylist(const std::string& playlistName) {
    // Constructs SQL to delete all songs associated with the playlist
    std::string sql1 = "DELETE FROM playlist_songs WHERE playlist_name = '" + playlistName + "'";
    
    // Constructs SQL to delete the playlist itself
    std::string sql2 = "DELETE FROM playlists WHERE name = '" + playlistName + "'";
    
    // Executes both DELETE statements and returns true if both succeed
    return executeSQL(sql1) && executeSQL(sql2);
}

/*****************************************************************************\
    Function: getPlaylistImage
    Description: Retrieves the image path for a specific playlist
    Parameters: 
        const std::string& playlistName -> name of the playlist
    Returns: 
        std::string -> path to the playlist's image
\*****************************************************************************/
std::string Backend::getPlaylistImage(const std::string& playlistName) {
    // Initializes empty string to store the result
    std::string result;
    
    // Constructs SQL SELECT statement to get image path
    std::string sql = "SELECT image_path FROM playlists WHERE name = '" + playlistName + "'";
    
    /**********************************\
        EXECUTE QUERY WITH LAMBDA
    \**********************************/
    
    // Initializes error message pointer
    char* errorMessage = nullptr;
    
    // Executes query with inline lambda callback to store image path
    sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** colName) -> int {
        // Checks if we have a valid result and it's not NULL
        if (argc > 0 && argv[0]) {
            // Stores the image path in the result string
            *static_cast<std::string*>(data) = argv[0];
        }
        return 0;
    }, &result, &errorMessage);
    
    // Frees error message memory if any error occurred
    if (errorMessage) {
        sqlite3_free(errorMessage);
    }
    
    // Returns the image path (empty string if not found)
    return result;
}

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
bool Backend::saveDownloadedSong(const std::string& title, const std::string& artist,
                                 const std::string& duration, const std::string& dateAdded,
                                 const std::string& filePath, const std::string& thumbnailPath) {
    // Constructs SQL INSERT statement with OR REPLACE clause for updating existing songs
    std::string sql = "INSERT OR REPLACE INTO downloaded_songs (file_path, title, artist, duration, date_added, thumbnail_path) "
                      "VALUES ('" + filePath + "', '" + title + "', '" + artist + "', '" 
                      + duration + "', '" + dateAdded + "', '" + thumbnailPath + "')";
    
    // Executes the SQL statement and returns result
    return executeSQL(sql);
}

/*****************************************************************************\
    Function: getAllDownloadedSongs
    Description: Retrieves all downloaded songs from the database
    Parameters: nothing
    Returns: 
        std::vector<DownloadedSong> -> vector containing all downloaded songs
\*****************************************************************************/
std::vector<DownloadedSong> Backend::getAllDownloadedSongs() {
    // Creates empty vector to store retrieved songs
    std::vector<DownloadedSong> songs;
    
    // Constructs SQL SELECT statement to get all downloaded songs
    std::string sql = "SELECT title, artist, duration, date_added, file_path, thumbnail_path FROM downloaded_songs";
    
    /**********************************\
        EXECUTE QUERY WITH LAMBDA
    \**********************************/
    
    // Initializes error message pointer
    char* errorMessage = nullptr;
    
    // Executes query with inline lambda callback to populate songs vector
    int statusCode = sqlite3_exec(db, sql.c_str(), 
        [](void* data, int argc, char** argv, char** colName) -> int {
            // Casts void pointer to vector of DownloadedSong objects
            auto* songs = static_cast<std::vector<DownloadedSong>*>(data);
            DownloadedSong song;
            
            // Ensures we have all 6 required columns
            if (argc >= 6) {
                // Stores all song metadata (empty string if NULL)
                song.title = argv[0] ? argv[0] : "";
                song.artist = argv[1] ? argv[1] : "";
                song.duration = argv[2] ? argv[2] : "";
                song.dateAdded = argv[3] ? argv[3] : "";
                song.filePath = argv[4] ? argv[4] : "";
                song.thumbnailPath = argv[5] ? argv[5] : "";
                songs->push_back(song);
            }
            return 0;
        }, &songs, &errorMessage);
    
    // Checks if query execution failed
    if (statusCode != SQLITE_OK) {
        std::cout << "[WARNING] Query failed: " << errorMessage << "\n";
        
        // Frees error message memory
        sqlite3_free(errorMessage);
    }
    
    // Returns vector of downloaded songs (empty if query failed)
    return songs;
}

/*****************************************************************************\
    Function: removeDownloadedSong
    Description: Removes a downloaded song from the database
    Parameters: 
        const std::string& filePath -> path of the song file to remove
    Returns: 
        bool -> true if removal successful, false otherwise
\*****************************************************************************/
bool Backend::removeDownloadedSong(const std::string& filePath) {
    // Constructs SQL DELETE statement to remove song by file path
    std::string sql = "DELETE FROM downloaded_songs WHERE file_path = '" + filePath + "'";
    
    // Executes the SQL statement and returns result
    return executeSQL(sql);
}
