/*****************************
    Author: Karam Zako
    Description: API Manager for yt-dlp operations (search and download)

******************************/

#ifndef YTDLPMANAGER_H
#define YTDLPMANAGER_H

// System Libraries
#include <string>
#include <iostream>
#include <vector>
#include <QMediaPlayer>
#include <QAudioOutput>

struct SongResult {
    std::string id;           // Video ID
    std::string title;        // Song title
    std::string uploader;     // Artist/Channel name
    std::string duration;     // Duration in seconds
    std::string thumbnail;    // Thumbnail URL
    std::string url;          // Video URL
};

class YTDLPManager {
private:
    std::string ytdlpPath = "..\\APIs\\YTDLP\\yt-dlp.exe";
    std::string searchResultsFile = "data\\search_results.json";
    std::string outputFolder = "data\\Downloads\\";

    QMediaPlayer* mediaPlayer;
    QAudioOutput* audioOutput;

public:
    YTDLPManager();
    ~YTDLPManager();

    // Updates the paths
    void setPaths(const std::string& ytdlp,
                  const std::string& searchFile,
                  const std::string& outputDir);
    
    // Search for songs on YouTube and return results
    std::vector<SongResult> searchSongs(const std::string& query, int maxResults = 10);
    
    // Download a song by URL to specified output path
    bool downloadSong(const std::string& url, const std::string& outputPath);
    
    // Get song info by URL
    SongResult getSongInfo(const std::string& url);

    // Play a song by URL
    void playSong(const std::string& url);
    void playSong();
    void stopSong();

    QMediaPlayer* getMediaPlayer() { return mediaPlayer; }

};

#endif
