#ifndef PLAYLISTITEMWIDGET_H
#define PLAYLISTITEMWIDGET_H

#include <QWidget>

namespace Ui {
class PlaylistItemWidget;
}

class PlaylistItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistItemWidget(QWidget *parent = nullptr);
    ~PlaylistItemWidget();

    void setName(const QString &titleName);
    void setArtist(const QString &tittleArtist);
    void setDuration(const QString &titleDuration);
    void setDate(const QString &titleDate);
    void setIcon(const QPixmap &pixmap);

private:
    Ui::PlaylistItemWidget *ui;
};

#endif
