#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QLabel>

#include "APIs/YTDLP/YTDLPManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(YTDLPManager &manager, QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void updateProgress(int position);
    void updateDuration(int duration);

    void on_searchButton_clicked();

    void on_searchList_customContextMenuRequested(const QPoint &pos);

    void on_stopButton_clicked();

    void loadThumbnail(const QString &url, QLabel *label);

private:
    Ui::MainWindow *ui;
    YTDLPManager &ytdlpManager;
    QNetworkAccessManager *networkManager;

    void addSong(const QString &titleName,
                 const QString &titleArtist,
                 const QString &titleDuration,
                 const QString &titleDate,
                 const QPixmap &icon);

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
};
#endif // MAINWINDOW_H
