/*****************************************************************************\
    Class: YTDLPManager.h
    Author: Karam Zako
    Date: Dec. 8, 2025
    Description: Header file for YTDLPManager class which manages YouTube downloads
        and playback using yt-dlp. Provides search functionality, audio downloading,
        streaming playback, and image conversion capabilities.
\*****************************************************************************/

// Checks if YTDLPMANAGER_H is not defined & prevents multiple inclusions
#ifndef YTDLPMANAGER_H
// Defines the file so that future "include" statements are skipped
#define YTDLPMANAGER_H

/**********************************\
    STANDARD LIBRARY INCLUDES
\**********************************/
#include <string>
#include <iostream>
#include <vector>

/**********************************\
    QT FRAMEWORK INCLUDES
\**********************************/
#include <QMediaPlayer>
#include <QAudioOutput>

/**********************************\
    DATA STRUCTURES
\**********************************/

/*****************************************************************************\
    Struct: SongResult
    Description: Stores metadata for a YouTube video/song search result
\*****************************************************************************/
struct SongResult {
    std::string id;           // Unique YouTube video ID (e.g., "dQw4w9WgXcQ")
    std::string title;        // Title of the video/song
    std::string uploader;     // Channel name or artist who uploaded the video
    std::string duration;     // Duration in seconds (stored as string)
    std::string thumbnail;    // URL to video thumbnail image (used for album art)
    std::string url;          // Full YouTube URL (e.g., "https://youtube.com/watch?v=...")
};

/**********************************\
    YTDLPMANAGER CLASS DEFINITION
\**********************************/

/*****************************************************************************\
    Class: YTDLPManager
    Description: Manages all YouTube-related operations including searching,
        downloading audio, streaming playback, and thumbnail conversion using
        yt-dlp command-line tool and Qt multimedia framework.
\*****************************************************************************/
class YTDLPManager {
private:
    /**********************************\
        PRIVATE MEMBERS
    \**********************************/
    
    // Path to yt-dlp executable (can be absolute or relative to application)
    std::string ytdlpPath = "..\\APIs\\YTDLP\\yt-dlp.exe";
    
    // Path to JSON file where search results are temporarily stored
    std::string searchResultsFile = "data\\search_results.json";
    
    // Directory path where downloaded songs will be saved
    std::string outputFolder = "data\\Downloads\\";

    // Qt media player instance for audio playback
    QMediaPlayer* mediaPlayer;
    
    // Qt audio output instance for controlling audio output device
    QAudioOutput* audioOutput;

public:
    /**********************************\
        PUBLIC CONSTRUCTORS & DESTRUCTORS
    \**********************************/
    
    /*****************************************************************************\
        Function: YTDLPManager (Constructor)
        Description: Initializes YTDLPManager with default paths and sets up
            media player for audio playback
        Parameters: nothing
        Returns: nothing
    \*****************************************************************************/
    YTDLPManager();
    
    /*****************************************************************************\
        Function: ~YTDLPManager (Destructor)
        Description: Cleans up media player resources when object is destroyed
        Parameters: nothing
        Returns: nothing
    \*****************************************************************************/
    ~YTDLPManager();

    /**********************************\
        PATH CONFIGURATION
    \**********************************/
    
    /*****************************************************************************\
        Function: setPaths
        Description: Configures custom paths for yt-dlp executable, search results
            file, and output directory for downloads
        Parameters: 
            const std::string& ytdlp -> path to yt-dlp.exe executable
            const std::string& searchFile -> path to JSON search results file
            const std::string& outputDir -> directory for downloaded audio files
        Returns: nothing
    \*****************************************************************************/
    void setPaths(const std::string& ytdlp,
                  const std::string& searchFile,
                  const std::string& outputDir);
    
    /**********************************\
        SEARCH AND DOWNLOAD OPERATIONS
    \**********************************/
    
    /*****************************************************************************\
        Function: searchSongs
        Description: Searches YouTube for songs matching the query and returns
            metadata including title, uploader, duration, and thumbnail URL.
            Uses yt-dlp's "ytsearch" feature to find videos without downloading.
        Parameters: 
            const std::string& query -> search query string (e.g., "Bohemian Rhapsody")
            int maxResults -> maximum number of search results to return (default: 10)
        Returns: 
            std::vector<SongResult> -> vector containing metadata for each result
    \*****************************************************************************/
    std::vector<SongResult> searchSongs(const std::string& query, int maxResults = 10);
    
    /*****************************************************************************\
        Function: downloadSong
        Description: Downloads audio from YouTube URL, extracts audio to MP3 format,
            and saves with thumbnail. Uses ffmpeg for audio conversion. Sanitizes
            filename to avoid Windows file system errors.
        Parameters: 
            const std::string& url -> YouTube URL to download from
            const std::string& title -> song title (used for filename, special chars removed)
        Returns: 
            bool -> true if download successful, false otherwise
    \*****************************************************************************/
    bool downloadSong(const std::string& url, const std::string& title);
    
    /*****************************************************************************\
        Function: getSongInfo
        Description: Retrieves detailed metadata for a single YouTube video by URL
            without downloading it. Returns information like title, duration, etc.
        Parameters: 
            const std::string& url -> YouTube URL to get information for
        Returns: 
            SongResult -> struct containing video metadata
    \*****************************************************************************/
    SongResult getSongInfo(const std::string& url);

    /**********************************\
        IMAGE CONVERSION
    \**********************************/
    
    /*****************************************************************************\
        Function: convertImageToPng
        Description: Converts an image file to PNG format using Qt's QImage.
            Supports various input formats (JPEG, WEBP, etc.). Used for converting
            downloaded thumbnails to a consistent format.
        Parameters: 
            const std::string& inputFilePath -> path to source image to convert
            const std::string& outputFilePath -> path where PNG will be saved
        Returns: 
            bool -> true if conversion successful, false otherwise
    \*****************************************************************************/
    bool convertImageToPng(const std::string& inputFilePath, const std::string& outputFilePath);

    /**********************************\
        PLAYBACK CONTROL
    \**********************************/
    
    /*****************************************************************************\
        Function: playSong (with URL parameter)
        Description: Streams audio from YouTube URL without downloading. Extracts
            direct audio stream URL using yt-dlp and plays through QMediaPlayer.
            Allows instant playback without waiting for full download.
        Parameters: 
            const std::string& url -> YouTube URL to stream from
        Returns: nothing
    \*****************************************************************************/
    void playSong(const std::string& url);

    /*****************************************************************************\
        Function: playSong (no parameters)
        Description: Toggles playback state - pauses if currently playing,
            resumes if currently paused. Alternative control method.
        Parameters: nothing
        Returns: nothing
    \*****************************************************************************/
    void playSong();

    /*****************************************************************************\
        Function: stopSong
        Description: Toggles playback state between play and pause. If currently
            playing, pauses the audio; if paused, resumes playback.
        Parameters: nothing
        Returns: nothing
    \*****************************************************************************/
    void stopSong();
    
    /*****************************************************************************\
        Function: getMediaPlayer
        Description: Returns pointer to internal QMediaPlayer instance. Allows
            external code to access player for advanced controls (volume, position,
            state queries, etc.).
        Parameters: nothing
        Returns: 
            QMediaPlayer* -> pointer to the media player instance
    \*****************************************************************************/
    QMediaPlayer* getMediaPlayer() { return mediaPlayer; }

};

#endif
