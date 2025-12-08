#include <vector>
#include <string>

#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QFile>
#include <QDebug>

#include "APIs/Json/json.hpp"
#include "YTDLPManager.h"

using json = nlohmann::json;
using namespace std;

YTDLPManager::YTDLPManager()
    : ytdlpPath("yt-dlp.exe"),      // temporary default
    searchResultsFile("search_results.json"),
    outputFolder("Downloads/")
{}

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

    // Prepare QProcess
    QProcess process;
    QStringList args;
    args << "--no-playlist"          // fetch full metadata
         << "--no-warnings"
         << "--skip-download"
         << "--dump-single-json"
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

    // Read JSON output
    QFile file(searchResultsFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[YTDLPManager] Failed to open JSON file:" << searchResultsFile;
        return results;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    if (jsonData.isEmpty()) {
        qDebug() << "[YTDLPManager] yt-dlp returned empty JSON!";
        return results;
    }

    // Parse JSON
    try {
        json data = json::parse(jsonData.constData());
        for (auto& item : data["entries"]) {
            SongResult s;
            s.title    = item.value("title", "Unknown");
            s.uploader = item.value("uploader", "Unknown");
            s.url      = item.value("webpage_url", "");

            // Get duration in seconds, default 0
            if (item.contains("duration") && item["duration"].is_number()) {
                s.duration = std::to_string(item["duration"].get<int>());
            } else {
                s.duration = "0";
            }

            results.push_back(s);
        }
    } catch (json::parse_error& e) {
        qDebug() << "[YTDLPManager] Failed to parse JSON:" << e.what();
    }

    return results;
}
