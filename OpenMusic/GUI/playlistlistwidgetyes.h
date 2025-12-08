#ifndef PLAYLISTLISTWIDGETYES_H
#define PLAYLISTLISTWIDGETYES_H

#include <QWidget>

namespace Ui {
class PlaylistListWidgetYes;
}

class PlaylistListWidgetYes : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistListWidgetYes(QWidget *parent = nullptr);
    ~PlaylistListWidgetYes();
    void setNamePlaylist(const QString &titleName);
    void setNumSong(const QString &titleNum);
    void setIconPlaylist(const QPixmap &pixmap);

private:
    Ui::PlaylistListWidgetYes *ui;
};

#endif // PLAYLISTLISTWIDGETYES_H
