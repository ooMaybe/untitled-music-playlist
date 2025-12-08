// User libraries
#include "Backend.h"

#include <QDebug>

Backend::~Backend(){
    closeDatabase();
}

void Backend::initialize(std::string &dbPath){
    db = nullptr;
    bool success = openDatabase(dbPath);
    if (success){
        qDebug() << "[Database] Sucessfully initialized.\n";
        createTables();
    }else{
        qDebug() << "[Database] Could not be created.\n";
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
    char* errorMessage = nullptr;
    int statusCode = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);    
    if (statusCode != SQLITE_OK){
        std::cout << "[WARNING] SQL execution failed! Error message: " << errorMessage << "\n";
        sqlite3_free(errorMessage);
        return false;   
    }
    
    return true;
}

int Backend::selectCallback(void *data, int argc, char **argv, char **colName) {
    auto *results = static_cast<std::vector<PlaylistSong>*>(data);
    PlaylistSong song;
    
    for (int i = 0; i < argc; i++) {
        std::string column = colName[i];
        std::string value = argv[i] ? argv[i] : "";
        
        if (column == "id") song.id = std::stoi(value);
        else if (column == "title") song.title = value;
        else if (column == "artist") song.artist = value;
        else if (column == "duration") song.duration = value;
        else if (column == "date_added") song.dateAdded = value;
        else if (column == "file_path") song.filePath = value;
        else if (column == "thumbnail_path") song.thumbnailPath = value;
    }
    
    results->push_back(song);
    return 0;
}

bool Backend::createTables() {
    std::string createPlaylists = 
        "CREATE TABLE IF NOT EXISTS playlists ("
        "name TEXT PRIMARY KEY, "
        "image_path TEXT NOT NULL)";
    
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
    
    std::string createDownloadedSongs = 
        "CREATE TABLE IF NOT EXISTS downloaded_songs ("
        "file_path TEXT PRIMARY KEY, "
        "title TEXT NOT NULL, "
        "artist TEXT, "
        "duration TEXT, "
        "date_added TEXT, "
        "thumbnail_path TEXT)";
    
    return executeSQL(createPlaylists) && executeSQL(createSongs) && executeSQL(createDownloadedSongs);
}

bool Backend::savePlaylist(const std::string& name, const std::string& imagePath) {
    std::string sql = "INSERT OR REPLACE INTO playlists (name, image_path) VALUES ('" 
                      + name + "', '" + imagePath + "')";
    return executeSQL(sql);
}

bool Backend::addSongToPlaylist(const std::string& playlistName, const std::string& title,
                                const std::string& artist, const std::string& duration,
                                const std::string& dateAdded, const std::string& filePath,
                                const std::string& thumbnailPath) {
    std::string sql = "INSERT INTO playlist_songs (playlist_name, title, artist, duration, date_added, file_path, thumbnail_path) "
                      "VALUES ('" + playlistName + "', '" + title + "', '" + artist + "', '" 
                      + duration + "', '" + dateAdded + "', '" + filePath + "', '" + thumbnailPath + "')";
    return executeSQL(sql);
}

std::vector<PlaylistSong> Backend::getPlaylistSongs(const std::string& playlistName) {
    std::vector<PlaylistSong> songs;
    std::string sql = "SELECT * FROM playlist_songs WHERE playlist_name = '" + playlistName + "'";
    
    char* errorMessage = nullptr;
    int statusCode = sqlite3_exec(db, sql.c_str(), selectCallback, &songs, &errorMessage);
    
    if (statusCode != SQLITE_OK) {
        std::cout << "[WARNING] Query failed: " << errorMessage << "\n";
        sqlite3_free(errorMessage);
    }
    
    return songs;
}

std::vector<Playlist> Backend::getAllPlaylists() {
    std::vector<Playlist> playlists;
    std::string sql = "SELECT name, image_path FROM playlists";
    
    char* errorMessage = nullptr;
    int statusCode = sqlite3_exec(db, sql.c_str(), 
        [](void* data, int argc, char** argv, char** colName) -> int {
            auto* playlists = static_cast<std::vector<Playlist>*>(data);
            Playlist p;
            if (argc >= 2) {
                p.name = argv[0] ? argv[0] : "";
                p.imagePath = argv[1] ? argv[1] : "";
                playlists->push_back(p);
            }
            return 0;
        }, &playlists, &errorMessage);
    
    if (statusCode != SQLITE_OK) {
        std::cout << "[WARNING] Query failed: " << errorMessage << "\n";
        sqlite3_free(errorMessage);
    }
    
    return playlists;
}

bool Backend::removeSongFromPlaylist(const std::string& playlistName, int songId) {
    std::string sql = "DELETE FROM playlist_songs WHERE id = " + std::to_string(songId) 
                      + " AND playlist_name = '" + playlistName + "'";
    return executeSQL(sql);
}

bool Backend::deletePlaylist(const std::string& playlistName) {
    std::string sql1 = "DELETE FROM playlist_songs WHERE playlist_name = '" + playlistName + "'";
    std::string sql2 = "DELETE FROM playlists WHERE name = '" + playlistName + "'";
    return executeSQL(sql1) && executeSQL(sql2);
}

std::string Backend::getPlaylistImage(const std::string& playlistName) {
    std::string result;
    std::string sql = "SELECT image_path FROM playlists WHERE name = '" + playlistName + "'";
    
    char* errorMessage = nullptr;
    sqlite3_exec(db, sql.c_str(), [](void* data, int argc, char** argv, char** colName) -> int {
        if (argc > 0 && argv[0]) {
            *static_cast<std::string*>(data) = argv[0];
        }
        return 0;
    }, &result, &errorMessage);
    
    if (errorMessage) {
        sqlite3_free(errorMessage);
    }
    
    return result;
}

bool Backend::saveDownloadedSong(const std::string& title, const std::string& artist,
                                 const std::string& duration, const std::string& dateAdded,
                                 const std::string& filePath, const std::string& thumbnailPath) {
    std::string sql = "INSERT OR REPLACE INTO downloaded_songs (file_path, title, artist, duration, date_added, thumbnail_path) "
                      "VALUES ('" + filePath + "', '" + title + "', '" + artist + "', '" 
                      + duration + "', '" + dateAdded + "', '" + thumbnailPath + "')";
    return executeSQL(sql);
}

std::vector<DownloadedSong> Backend::getAllDownloadedSongs() {
    std::vector<DownloadedSong> songs;
    std::string sql = "SELECT title, artist, duration, date_added, file_path, thumbnail_path FROM downloaded_songs";
    
    char* errorMessage = nullptr;
    int statusCode = sqlite3_exec(db, sql.c_str(), 
        [](void* data, int argc, char** argv, char** colName) -> int {
            auto* songs = static_cast<std::vector<DownloadedSong>*>(data);
            DownloadedSong song;
            if (argc >= 6) {
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
    
    if (statusCode != SQLITE_OK) {
        std::cout << "[WARNING] Query failed: " << errorMessage << "\n";
        sqlite3_free(errorMessage);
    }
    
    return songs;
}

bool Backend::removeDownloadedSong(const std::string& filePath) {
    std::string sql = "DELETE FROM downloaded_songs WHERE file_path = '" + filePath + "'";
    return executeSQL(sql);
}
