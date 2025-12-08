#include <iostream>
#include <filesystem>
#include <QApplication>
#include <QDir>

// Project defined libraries
#include "GUI/mainwindow.h"
#include "Backend/Backend.h"
#include "Backend/FileManager.h"
#include "APIs/YTDLP/YTDLPManager.h"

using namespace std;
namespace fs = std::filesystem;

Backend backend;
FileManager fileManager;
YTDLPManager ytdlpManager;

void handleSetup();
int handleUI(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    handleSetup();

    MainWindow w(ytdlpManager);
    w.show();

    return app.exec();
}

void handleSetup() {
    std::cout << "[Main] Starting setup...\n";

    // Base dir = folder of the exe (inside build dir)
    QDir dir(QCoreApplication::applicationDirPath());

    fileManager = FileManager();
    fileManager.createFolder(dir.filePath("data").toStdString());
    fileManager.createFolder(dir.filePath("data/Downloads").toStdString());

    // Navigate to project root using QDir
    QDir projectRoot = dir;
    projectRoot.cdUp();  // debug
    projectRoot.cdUp();  // Desktop_Qt_6_9_2_MinGW_64_bit-Debug
    projectRoot.cdUp();  // build
    // Now we're at OpenMusic folder

    QString sourceFilePath = projectRoot.filePath("APIs/YTDLP/yt-dlp.exe");
    QString targetDirPath = dir.filePath("APIs/YTDLP");
    QString targetFilePath = dir.filePath("APIs/YTDLP/yt-dlp.exe");

    // Create target directory
    QDir().mkpath(targetDirPath);

    if (!QFile::exists(targetFilePath)) {
        if (QFile::exists(sourceFilePath)) {
            if (QFile::copy(sourceFilePath, targetFilePath)) {
                std::cout << "[Setup] Copied yt-dlp.exe to runtime API folder.\n";
            } else {
                std::cerr << "[Setup] Failed to copy yt-dlp.exe!\n";
            }
        } else {
            std::cerr << "[Setup] Source yt-dlp.exe not found!\n";
        }
    } else {
        std::cout << "[Setup] yt-dlp.exe already exists at target.\n";
    }

    std::string ytdlpPath =dir.filePath("APIs/YTDLP/yt-dlp.exe").toStdString();
    std::string searchResultsFile =dir.filePath("data/search_results.json").toStdString();
    std::string outputFolder =dir.filePath("data/Downloads/").toStdString();
    std::string databasePath = dir.filePath("data/OpenMusic.db").toStdString();

    ytdlpManager.setPaths(ytdlpPath, searchResultsFile, outputFolder);

    backend = Backend();
    backend.initialize(databasePath);

    std::cout << "[Main] Setup complete.\n";
}
