/*****************************************************************************\
    Class: YTDLPManager.cpp
    Description: YTDLPManager class implementation for managing YouTube downloads
        and playback using yt-dlp. Handles searching songs, downloading audio,
        streaming playback, and image conversion.
\*****************************************************************************/

/**********************************\
    STANDARD LIBRARY INCLUDES
\**********************************/
#include <vector>
#include <string>

/**********************************\
    QT FRAMEWORK INCLUDES
\**********************************/
#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QRegularExpression>

/**********************************\
    PROJECT INCLUDES
\**********************************/
#include "APIs/Json/json.hpp"
#include "YTDLPManager.h"

using json = nlohmann::json;
using namespace std;

/*****************************************************************************\
    Function: YTDLPManager (Constructor)
    Description: Initializes the YTDLPManager with default paths and sets up
        media player for audio playback
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
YTDLPManager::YTDLPManager()
    : ytdlpPath("yt-dlp.exe"),      // Temporary default path to yt-dlp executable
    searchResultsFile("search_results.json"),  // Default JSON file for search results
    outputFolder("Downloads/")       // Default folder for downloaded songs
{
    /**********************************\
        MEDIA PLAYER INITIALIZATION
    \**********************************/
    
    // Creates a new QMediaPlayer instance for audio playback
    mediaPlayer = new QMediaPlayer();
    
    // Creates a new QAudioOutput instance for audio output control
    audioOutput = new QAudioOutput();
    
    // Connects the audio output to the media player
    mediaPlayer->setAudioOutput(audioOutput);
}

/*****************************************************************************\
    Function: ~YTDLPManager (Destructor)
    Description: Cleans up media player resources when object is destroyed
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
YTDLPManager::~YTDLPManager() {
    // Deletes the media player to free memory
    delete mediaPlayer;
    
    // Deletes the audio output to free memory
    delete audioOutput;
}

/*****************************************************************************\
    Function: setPaths
    Description: Configures custom paths for yt-dlp executable, search results
        file, and output directory
    Parameters: 
        const std::string& ytdlp -> path to yt-dlp executable
        const std::string& searchFile -> path to JSON search results file
        const std::string& outputDir -> directory for downloaded songs
    Returns: nothing
\*****************************************************************************/
void YTDLPManager::setPaths(const std::string& ytdlp,
                            const std::string& searchFile,
                            const std::string& outputDir)
{
    // Updates the path to yt-dlp executable
    ytdlpPath         = ytdlp;
    
    // Updates the path to search results JSON file
    searchResultsFile = searchFile;
    
    // Updates the output directory for downloads
    outputFolder      = outputDir;
}

/*****************************************************************************\
    Function: searchSongs
    Description: Searches YouTube for songs matching the query and returns
        metadata including title, uploader, duration, and thumbnail URL
    Parameters: 
        const std::string& query -> search query string (e.g., "Bohemian Rhapsody")
        int limit -> maximum number of results to return (default: varies)
    Returns: 
        std::vector<SongResult> -> vector containing song metadata for each result
\*****************************************************************************/
std::vector<SongResult> YTDLPManager::searchSongs(const std::string& query, int limit) {
    /**********************************\
        INITIALIZATION
    \**********************************/
    
    // Creates empty vector to store search results
    std::vector<SongResult> results;

    /**********************************\
        PATH SETUP
    \**********************************/
    
    // Gets the directory where the application executable is located
    QString exeDir = QCoreApplication::applicationDirPath();
    
    // Constructs absolute path to yt-dlp executable in APIs/YTDLP folder
    QString ytDlpPath = QDir(exeDir).filePath("APIs/YTDLP/yt-dlp.exe");
    
    // Constructs absolute path to search results JSON file in data folder
    QString searchResultsFile = QDir(exeDir).filePath("data/search_results.json");

    /**********************************\
        YT-DLP PROCESS CONFIGURATION
    \**********************************/
    
    // Creates QProcess object to run yt-dlp as external process
    QProcess process;
    
    // Creates list to hold command-line arguments for yt-dlp
    QStringList args;
    
    // ARGUMENT: --no-playlist -> Prevents downloading entire playlists, only individual videos
    args << "--no-playlist"
         // ARGUMENT: --no-warnings -> Suppresses warning messages for cleaner output
         << "--no-warnings"
         // ARGUMENT: --skip-download -> Only fetches metadata without downloading video/audio
         << "--skip-download"
         // ARGUMENT: -j -> Outputs one JSON object per line containing video metadata
         << "-j"
         // ARGUMENT: ytsearch[limit]:[query] -> Searches YouTube for [limit] results matching [query]
         // Example: "ytsearch5:Bohemian Rhapsody" searches for 5 results
         << QString("ytsearch%1:%2").arg(limit).arg(QString::fromStdString(query));

    // Sets the program to execute (yt-dlp executable path)
    process.setProgram(ytDlpPath);
    
    // Passes the argument list to the process
    process.setArguments(args);
    
    // Redirects standard output (JSON data) to the search results file
    process.setStandardOutputFile(searchResultsFile);
    
    // Merges stdout and stderr into single channel for unified output
    process.setProcessChannelMode(QProcess::MergedChannels);

    qDebug() << "[YTDLPManager] Running yt-dlp via QProcess:" << ytDlpPath;

    /**********************************\
        EXECUTE YT-DLP SEARCH
    \**********************************/
    
    // Starts the yt-dlp process asynchronously
    process.start();
    
    // Waits for process to complete with extended timeout for slow connections
    // -1 means wait indefinitely until process finishes
    if (!process.waitForFinished(-1)) {
        qDebug() << "[YTDLPManager] yt-dlp failed to run!";
        return results;  // Returns empty vector on failure
    }

    /**********************************\
        OPEN AND READ JSON FILE
    \**********************************/
    
    // Creates QFile object for the search results JSON file
    QFile file(searchResultsFile);
    
    // Opens file in read-only text mode
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[YTDLPManager] Failed to open JSON file:" << searchResultsFile;
        return results;  // Returns empty vector if file cannot be opened
    }

    /**********************************\
        PARSE JSON RESULTS LINE BY LINE
    \**********************************/
    
    // Creates text stream for reading file line by line
    QTextStream in(&file);
    
    // Iterates through each line in the JSON file
    while (!in.atEnd()) {
        // Reads one line (one JSON object per video result)
        QString line = in.readLine();
        
        // Skips empty lines
        if (line.isEmpty()) continue;

        try {
            // Parses the JSON line into a JSON object
            json item = json::parse(line.toStdString());

            /**********************************\
                EXTRACT SONG METADATA
            \**********************************/
            
            // Creates new SongResult object to store metadata
            SongResult s;
            
            // Extracts video ID (unique identifier)
            s.id = item.value("id", "");
            
            // Extracts video title, defaults to "Unknown" if missing
            s.title = item.value("title", "Unknown");
            
            // Extracts uploader/channel name, defaults to "Unknown" if missing
            s.uploader = item.value("uploader", "Unknown");
            
            // Extracts full YouTube URL to the video
            s.url = item.value("webpage_url", "");
            
            // Extracts thumbnail image URL (used for album art)
            s.thumbnail = item.value("thumbnail", "");

            /**********************************\
                EXTRACT DURATION
            \**********************************/
            
            // Checks if duration field exists and is a number
            if (item.contains("duration") && item["duration"].is_number()) {
                // Converts duration from integer seconds to string
                s.duration = std::to_string(item["duration"].get<int>());
            } else {
                // Sets duration to "0" if not available
                s.duration = "0";
            }

            // Adds the song result to the results vector
            results.push_back(s);
            
        } catch (json::parse_error& e) {
            // Catches and logs JSON parsing errors without stopping the loop
            qDebug() << "[YTDLPManager] Failed to parse JSON line:" << e.what();
        }
    }
    
    // Closes the file after reading all lines
    file.close();

    // Returns the vector containing all successfully parsed song results
    return results;
}

/*****************************************************************************\
    Function: downloadSong
    Description: Downloads audio from YouTube URL, converts to MP3, and saves
        with thumbnail. Uses ffmpeg for audio extraction and conversion.
    Parameters: 
        const std::string& url -> YouTube URL to download (e.g., "https://youtube.com/watch?v=...")
        const std::string& title -> song title used for filename (special chars removed)
    Returns: 
        bool -> true if download successful, false otherwise
\*****************************************************************************/
bool YTDLPManager::downloadSong(const std::string& url, const std::string& title){
    qDebug() << "[YTDLPManager] Downloading song:" << QString::fromStdString(url);
    
    // Displays warning message to user indicating download has started
    QMessageBox::warning(nullptr, "Warning!", QString::fromStdString("Downloading the song: " + title));

    /**********************************\
        PATH SETUP
    \**********************************/
    
    // Constructs absolute path to yt-dlp executable
    QString ytDlpPath = QDir(QCoreApplication::applicationDirPath()).filePath("APIs/YTDLP/yt-dlp.exe");
    
    // Constructs absolute path to Downloads folder where songs will be saved
    QString downloadFolder = QDir(QCoreApplication::applicationDirPath()).filePath("data/Downloads");
    
    // Constructs absolute path to ffmpeg executable (required for audio conversion)
    QString ffmpegPath = QDir(QCoreApplication::applicationDirPath()).filePath("APIs/YTDLP/ffmpeg.exe");

    /**********************************\
        SANITIZE FILENAME
    \**********************************/
    
    // CRITICAL: Removes special characters that are invalid in Windows filenames
    // Characters removed: < > : " / \ | ? *
    // This prevents Windows file system errors during download
    QString safeTitle = QString::fromStdString(title);
    safeTitle.replace(QRegularExpression("[<>:\"/\\\\|?*]"), "");

    /**********************************\
        YT-DLP DOWNLOAD ARGUMENTS
    \**********************************/
    
    // Constructs output file path with extension placeholder (replaced by yt-dlp)
    // %(ext)s will be replaced with actual extension (e.g., "mp3")
    QString outputPath = QDir(downloadFolder).filePath(safeTitle + ".%(ext)s");
    
    // Creates list of command-line arguments for yt-dlp download
    QStringList args;
    
    // ARGUMENT: -f bestaudio -> Selects the best available audio quality stream
    args << "-f" << "bestaudio"
         // ARGUMENT: --extract-audio -> Extracts only audio, discards video
         << "--extract-audio"
         // ARGUMENT: --audio-format mp3 -> Converts extracted audio to MP3 format
         << "--audio-format" << "mp3"
         // ARGUMENT: --audio-quality 0 -> Sets highest quality (0 = best, 9 = worst)
         << "--audio-quality" << "0"
         // ARGUMENT: --ffmpeg-location [path] -> Specifies where to find ffmpeg executable
         << "--ffmpeg-location" << ffmpegPath
         // ARGUMENT: --write-thumbnail -> Downloads video thumbnail image
         << "--write-thumbnail"
         // ARGUMENT: --convert-thumbnails png -> Converts thumbnail to PNG format
         << "--convert-thumbnails" << "png"
         // ARGUMENT: -o [path] -> Specifies output file path and naming template
         << "-o" << outputPath
         // FINAL ARGUMENT: The YouTube URL to download from
         << QString::fromStdString(url);

    /**********************************\
        EXECUTE DOWNLOAD PROCESS
    \**********************************/
    
    // Creates QProcess object to run yt-dlp download
    QProcess downloadProcess;
    
    // Sets yt-dlp executable as the program to run
    downloadProcess.setProgram(ytDlpPath);
    
    // Passes all download arguments to the process
    downloadProcess.setArguments(args);
    
    // Starts the download process asynchronously
    downloadProcess.start();

    qDebug() << "[YTDLPManager] Downloading to:" << outputPath;

    /**********************************\
        WAIT FOR DOWNLOAD COMPLETION
    \**********************************/
    
    // Waits indefinitely (-1 timeout) for download to complete
    // Returns false if process fails to finish properly
    if (!downloadProcess.waitForFinished(-1)) {
        qDebug() << "[YTDLPManager] Download failed!";
        return false;
    }

    /**********************************\
        CHECK DOWNLOAD STATUS
    \**********************************/
    
    // Checks if process exited with success code (0 = success)
    if (downloadProcess.exitCode() == 0) {
        qDebug() << "[YTDLPManager] Download complete!";
        return true;
    } else {
        // Logs error message from yt-dlp if download failed
        qDebug() << "[YTDLPManager] Download error:" << downloadProcess.readAllStandardError();
        return false;
    }
}

/*****************************************************************************\
    Function: playSong
    Description: Streams audio from YouTube URL without downloading. Extracts
        direct audio stream URL and plays through QMediaPlayer.
    Parameters: 
        const std::string& url -> YouTube URL to stream (e.g., "https://youtube.com/watch?v=...")
    Returns: nothing
\*****************************************************************************/
void YTDLPManager::playSong(const std::string& url) {
    qDebug() << "[YTDLPManager] Playing song:" << QString::fromStdString(url);

    /**********************************\
        PATH SETUP
    \**********************************/
    
    // Gets application directory for locating yt-dlp executable
    QString exeDir = QCoreApplication::applicationDirPath();
    
    // Constructs path to yt-dlp executable
    QString ytDlpPath = QDir(exeDir).filePath("APIs/YTDLP/yt-dlp.exe");

    /**********************************\
        YT-DLP STREAM URL EXTRACTION
    \**********************************/
    
    // Creates list of arguments to extract direct stream URL
    QStringList args;
    
    // ARGUMENT: -f bestaudio -> Selects best audio quality stream
    args << "-f" << "bestaudio"
         // ARGUMENT: --get-url -> Prints direct URL to audio stream (no download)
         << "--get-url"
         // FINAL ARGUMENT: YouTube URL to process
         << QString::fromStdString(url);

    /**********************************\
        EXECUTE URL EXTRACTION PROCESS
    \**********************************/
    
    // Creates process to run yt-dlp for URL extraction
    QProcess getUrlProcess;
    
    // Sets yt-dlp as the program to execute
    getUrlProcess.setProgram(ytDlpPath);
    
    // Passes arguments to the process
    getUrlProcess.setArguments(args);
    
    // Starts the process to extract stream URL
    getUrlProcess.start();
    
    // Waits for process to complete (should be quick, just extracting URL)
    getUrlProcess.waitForFinished();

    /**********************************\
        READ AND VALIDATE STREAM URL
    \**********************************/
    
    // Reads the direct stream URL from process output and removes whitespace
    QString streamUrl = QString::fromUtf8(getUrlProcess.readAllStandardOutput()).trimmed();

    // Checks if URL extraction failed (empty result)
    if (streamUrl.isEmpty()) {
        qDebug() << "[YTDLPManager] Failed to get stream URL!";
        return;  // Exits function if no URL obtained
    }

    qDebug() << "[YTDLPManager] Stream URL:" << streamUrl;

    /**********************************\
        PLAY AUDIO STREAM
    \**********************************/
    
    // Sets the direct audio stream URL as media player source
    mediaPlayer->setSource(QUrl(streamUrl));
    
    // Starts playback of the audio stream
    mediaPlayer->play();
}

/*****************************************************************************\
    Function: stopSong
    Description: Toggles playback state - pauses if playing, resumes if paused
    Parameters: nothing
    Returns: nothing
\*****************************************************************************/
void YTDLPManager::stopSong() {
    /**********************************\
        TOGGLE PLAYBACK STATE
    \**********************************/
    
    // Checks if media is currently playing
    if (mediaPlayer->isPlaying()){
        // Pauses playback if currently playing
        mediaPlayer->pause();
    }else{
        // Resumes playback if currently paused
        mediaPlayer->play();
    }
}

/*****************************************************************************\
    Function: convertImageToPng
    Description: Converts an image file to PNG format using Qt's QImage class.
        Supports various input formats (JPEG, WEBP, etc.).
    Parameters: 
        const std::string& inputFilePath -> path to source image file to convert
        const std::string& outputFilePath -> path where PNG file will be saved
    Returns: 
        bool -> true if conversion successful, false otherwise
\*****************************************************************************/
bool YTDLPManager::convertImageToPng(const std::string& inputFilePath, const std::string& outputFilePath) {
    /**********************************\
        CONVERT PATH STRINGS
    \**********************************/
    
    // Converts C++ string to Qt QString for input path
    QString qInputPath = QString::fromStdString(inputFilePath);
    
    // Converts C++ string to Qt QString for output path
    QString qOutputPath = QString::fromStdString(outputFilePath);

    /**********************************\
        LOAD SOURCE IMAGE
    \**********************************/
    
    // Creates QImage object for loading and manipulating images
    QImage image;
    
    // Attempts to load the image from the input file path
    if (!image.load(qInputPath)) {
        qDebug() << "[YTDLPManager] Failed to load image for conversion:" << qInputPath;
        return false;  // Returns false if image cannot be loaded
    }

    /**********************************\
        SAVE AS PNG FORMAT
    \**********************************/
    
    // Saves the loaded image as PNG format to output path
    // "PNG" parameter specifies the output format
    if (!image.save(qOutputPath, "PNG")) {
        qDebug() << "[YTDLPManager] Failed to save image as PNG:" << qOutputPath;
        return false;  // Returns false if save operation fails
    }

    qDebug() << "[YTDLPManager] Image converted successfully to PNG:" << qOutputPath;
    
    // Returns true on successful conversion
    return true;
}
