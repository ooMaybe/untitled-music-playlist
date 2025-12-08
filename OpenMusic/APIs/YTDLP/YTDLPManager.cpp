#include <vector>
#include <string>

#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>

#include "APIs/Json/json.hpp"
#include "YTDLPManager.h"

using json = nlohmann::json;
using namespace std;

YTDLPManager::YTDLPManager()
    : ytdlpPath("yt-dlp.exe"),      // temporary default
    searchResultsFile("search_results.json"),
    outputFolder("Downloads/")
{
    mediaPlayer = new QMediaPlayer();
    audioOutput = new QAudioOutput();
    mediaPlayer->setAudioOutput(audioOutput);
}

YTDLPManager::~YTDLPManager() {
    delete mediaPlayer;
    delete audioOutput;
}

void YTDLPManager::setPaths(const std::string& ytdlp,
                            const std::string& searchFile,
                            const std::string& outputDir)
{
    ytdlpPath         = ytdlp;
    searchResultsFile = searchFile;
    outputFolder      = outputDir;
}

std::vector<SongResult> YTDLPManager::searchSongs(const std::string& query, int limit) {
    std::vector<SongResult> results;

    QString exeDir = QCoreApplication::applicationDirPath();
    QString ytDlpPath = QDir(exeDir).filePath("APIs/YTDLP/yt-dlp.exe");
    QString searchResultsFile = QDir(exeDir).filePath("data/search_results.json");

    QProcess process;
    QStringList args;
    args << "--no-playlist"
         << "--no-warnings"
         << "--skip-download"
         << "-j"  // Output one JSON object per line
         << QString("ytsearch%1:%2").arg(limit).arg(QString::fromStdString(query));

    process.setProgram(ytDlpPath);
    process.setArguments(args);
    process.setStandardOutputFile(searchResultsFile);
    process.setProcessChannelMode(QProcess::MergedChannels);

    qDebug() << "[YTDLPManager] Running yt-dlp via QProcess:" << ytDlpPath;

    process.start();
    if (!process.waitForFinished()) {
        qDebug() << "[YTDLPManager] yt-dlp failed to run!";
        return results;
    }

    QFile file(searchResultsFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[YTDLPManager] Failed to open JSON file:" << searchResultsFile;
        return results;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty()) continue;

        try {
            json item = json::parse(line.toStdString());

            SongResult s;
            s.id = item.value("id", "");
            s.title = item.value("title", "Unknown");
            s.uploader = item.value("uploader", "Unknown");
            s.url = item.value("webpage_url", "");
            s.thumbnail = item.value("thumbnail", "");

            // Get duration in seconds
            if (item.contains("duration") && item["duration"].is_number()) {
                s.duration = std::to_string(item["duration"].get<int>());
            } else {
                s.duration = "0";
            }

            results.push_back(s);
        } catch (json::parse_error& e) {
            qDebug() << "[YTDLPManager] Failed to parse JSON line:" << e.what();
        }
    }
    file.close();

    return results;
}

void YTDLPManager::playSong(const std::string& url) {
    qDebug() << "[YTDLPManager] Playing song:" << QString::fromStdString(url);

    QString exeDir = QCoreApplication::applicationDirPath();
    QString ytDlpPath = QDir(exeDir).filePath("APIs/YTDLP/yt-dlp.exe");

    QStringList args;
    args << "-f" << "bestaudio"
         << "--get-url"
         << QString::fromStdString(url);

    QProcess getUrlProcess;
    getUrlProcess.setProgram(ytDlpPath);
    getUrlProcess.setArguments(args);
    getUrlProcess.start();
    getUrlProcess.waitForFinished();

    QString streamUrl = QString::fromUtf8(getUrlProcess.readAllStandardOutput()).trimmed();

    if (streamUrl.isEmpty()) {
        qDebug() << "[YTDLPManager] Failed to get stream URL!";
        return;
    }

    qDebug() << "[YTDLPManager] Stream URL:" << streamUrl;

    // Play the audio stream
    mediaPlayer->setSource(QUrl(streamUrl));
    mediaPlayer->play();
}

void YTDLPManager::stopSong() {
    if (mediaPlayer->isPlaying()){
        mediaPlayer->pause();
    }else{
        mediaPlayer->play();
    }
}

void playSong(){ // add file here later

}
