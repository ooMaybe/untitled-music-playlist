#include <iostream>
#include <QApplication>

// Project defined libraries
#include "GUI/mainwindow.h"
#include "Backend/Backend.h"
#include "Backend/FileManager.h"
#include "APIs/YTDLP/YTDLPManager.h"

using namespace std;

Backend backend;
FileManager fileManager;
YTDLPManager ytdlpManager;

void handleSetup();
int handleUI(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    handleSetup();
    int exitCode = handleUI(argc, argv);

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

    return exitCode;
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

int handleUI(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
