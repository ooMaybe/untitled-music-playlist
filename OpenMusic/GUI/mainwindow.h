#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>
#include <QTreeWidget>

#include "APIs/YTDLP/YTDLPManager.h"
#include "Backend/Backend.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(YTDLPManager &manager, Backend &backend, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void updateProgress(int position);
    void updateDuration(int duration);

    void on_searchButton_clicked();

    void on_searchList_customContextMenuRequested(const QPoint &pos);
    void on_sideBarPlaylist_customContextMenuRequested(const QPoint &pos);

    void on_stopButton_clicked();

    void loadThumbnail(const QString &url, QLabel *label);
    void loadThumbnailForTreeItem(const QString &url, QTreeWidgetItem *item);  // ADD THIS

    void on_songsDownload_customContextMenuRequested(const QPoint &pos);
    void on_mainPlaylistTree_customContextMenuRequested(const QPoint &pos);
    void on_homeButton_clicked();

    void on_chooseImageButton_clicked();

    void on_createPlaylistButton_clicked();

    void loadData();

    void on_forwardTen_clicked();
    void on_backwardTen_clicked();
    
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    Ui::MainWindow *ui;
    YTDLPManager &ytdlpManager;
    Backend &backend;
    QNetworkAccessManager *networkManager;
    QString currentPlaylist;  // Track which playlist is currently open
    bool isPlayingFromFile;   // Track if playing from local file or web URL
    int currentSongIndex;     // Track current song position in playlist for auto-play

    void addSong(const QString &titleName,
                 const QString &titleArtist,
                 const QString &titleDuration,
                 const QString &titleDate,
                 const QPixmap &icon,
                 const QString &filePath);

    void addPlaylist(const QString &titleName,
                       const QString &titleNumSong,
                       const QPixmap &icon);

    void addSearch(const QString &titleName,
                   const QString &titleArtist,
                   const QString &titleDuration,
                   const QString &titleDate,
                   const QPixmap &icon,
                   const QString &url,
                   const QString &thumbnail);

    void addToPlaylistPage(const QString &titleName,
                                       const QString &titleArtist,
                                       const QString &titleDuration,
                                       const QString &titleDate,
                           const QPixmap &icon);

    void addSidePlaylist(const QString &titleName,
                                     const QPixmap &icon);
};
#endif // MAINWINDOW_H
