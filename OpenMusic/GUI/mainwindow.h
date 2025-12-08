#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void on_searchButton_clicked();

    void on_searchList_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    YTDLPManager &ytdlpManager;

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
                 const QPixmap &icon);
};
#endif // MAINWINDOW_H
