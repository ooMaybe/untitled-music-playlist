#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
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
