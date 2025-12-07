
#include <fstream>
#include <windows.h>

#include "YTDLPManager.h"

using json = nlohmann::json;
using namespace std;

YTDLPManager::YTDLPManager(){
    // Constructor implementation can be added here if needed
}

vector<SongResult> YTDLPManager::searchSongs(const string& query, int maxResults){
    // Construct the command to search YouTube using yt-dlp
    // Redirect output to a JSON file
    // --flat-playlist to get individual video entries
    // --no-playlist to avoid playlist processing
    // --no-warnings to suppress warnings
    // --skip-download to avoid downloading videos
    // --dump-single-json to output JSON data

    cout << "[YTDLPManager] Searching for: " << query << endl;

    string command = ytdlpPath + " --flat-playlist --no-playlist --no-warnings --skip-download --dump-single-json \"ytsearch" 
                     + to_string(maxResults) + ":" + query + "\" > " + searchResultsFile;
    FILE* pipe = _popen(command.c_str(), "r");

    cout << "[YTDLPManager] Search completed!\n";

    if (pipe){
        // Close the pipe after execution to avoid resource leaks
        _pclose(pipe);
    }

    cout << "[YTDLPManager] Attempting to open: " << searchResultsFile << "\n";
    vector<SongResult> results;
    ifstream file(searchResultsFile);

    if (!file.is_open()) {
        cerr << "[YTDLPManager] Failed to open JSON file: " << searchResultsFile << "\n";
        return results;
    }

    cout << "[YTDLPManager] File opened successfully!\n";
    
    // Proceed with reading and parsing the JSON file here
    try{
        json fileData = json::parse(file);

        if (!fileData.contains("entries") || !fileData["entries"].is_array()) {
            cerr << "[YTDLPManager] JSON data does not contain 'entries' array.\n";
            file.close();
            return results;
        }

        for (const auto& item : fileData["entries"]) {
            SongResult song;
            song.id = item.value("id", "");
            song.title = item.value("title", "");
            song.uploader = item.value("uploader", "");
            
            // Duration is a double in the JSON, convert to string
            double durationValue = item.value("duration", 0.0);
            song.duration = to_string(static_cast<int>(durationValue));
            
            // Thumbnail is in a thumbnails array, not a string field
            if (item.contains("thumbnails") && item["thumbnails"].is_array() && !item["thumbnails"].empty()) {
                song.thumbnail = item["thumbnails"][0].value("url", "");
            } else {
                song.thumbnail = "";
            }
            
            song.url = "https://www.youtube.com/watch?v=" + song.id;

            results.push_back(song);
        }

        file.close();
        cout << "[YTDLPManager] Parsed " << results.size() << " results from JSON file.\n";
    }catch(json::parse_error& ex){
        cerr << "[YTDLPManager] Failed to parse JSON data from file: " << searchResultsFile << ". Error: " << ex.what() << "\n";
        file.close();
        return results;
    }
    
    return results;
}