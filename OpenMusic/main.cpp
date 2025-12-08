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

    // ensures yt-dlp.exe is copied to apis folder
    fs::path targetDir = dir.filePath("APIs/YTDLP").toStdString();
    fs::create_directories(targetDir);  // make sure folder exists

    fs::path targetFile = targetDir / "yt-dlp.exe";
    fs::path sourceFile = fs::path("../../../APIs/YTDLP/yt-dlp.exe"); // adjust relative to your project

    if (!fs::exists(targetFile)) {
        if (fs::exists(sourceFile)) {
            fs::copy_file(sourceFile, targetFile, fs::copy_options::overwrite_existing);
            std::cout << "[Setup] Copied yt-dlp.exe to runtime API folder.\n";
        } else {
            std::cerr << "[Setup] Source yt-dlp.exe not found!\n";
        }
    }

    std::string ytdlpPath =
        dir.filePath("APIs/YTDLP/yt-dlp.exe").toStdString();
    std::string searchResultsFile =
        dir.filePath("data/search_results.json").toStdString();
    std::string outputFolder =
        dir.filePath("data/Downloads/").toStdString();

    ytdlpManager.setPaths(ytdlpPath, searchResultsFile, outputFolder);

    backend = Backend();
    backend.initialize();

    std::cout << "[Main] Setup complete.\n";
}
