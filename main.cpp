#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

// Project defined libraries
#include "Backend/Backend.h"
#include "Backend/FileManager.h"
#include "APIs/YTDLP/YTDLPManager.h"

using namespace std;

Backend backend;
FileManager fileManager;
YTDLPManager ytdlpManager;

void handleSetup();
void handleUI();

int main() {
    handleSetup();
    handleUI();

    return 0;
}

void handleSetup(){
    cout << "[Main] Starting setup...\n";
    
    // Initialize Backend and FileManager instances
    fileManager = FileManager();
    fileManager.createFolder("data");

    backend = Backend();
    backend.initialize();

    cout << "[Main] Setup complete.\n";
}

void handleUI(){
    cout << "Please enter a song to search: ";
    string songName;
    getline(cin, songName);

    // Search for the song
    vector<SongResult> results = ytdlpManager.searchSongs(songName, 10);
    
    // Display results
    if (results.empty()) {
        cout << "No results found.\n";
    } else {
        cout << "\nSearch Results:\n";
        cout << "===============================================\n";
        
        for (size_t i = 0; i < results.size(); i++) {
            cout << (i + 1) << ". " << results[i].title << "\n";
            cout << "   Artist: " << results[i].uploader << "\n";
            cout << "   Duration: " << results[i].duration << " seconds\n";
            cout << "   URL: " << results[i].url << "\n";
            cout << "-----------------------------------------------\n";
        }
    }
}