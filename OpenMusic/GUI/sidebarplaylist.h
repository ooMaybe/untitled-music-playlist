#ifndef SIDEBARPLAYLIST_H
#define SIDEBARPLAYLIST_H

#include <QWidget>

namespace Ui {
class sideBarPlaylist;
}

class sideBarPlaylist : public QWidget
{
    Q_OBJECT

public:
    explicit sideBarPlaylist(QWidget *parent = nullptr);
    ~sideBarPlaylist();
    void setName(const QString &titleName);
    void setIcon(const QPixmap &pixmap);

private:
    Ui::sideBarPlaylist *ui;
};

#endif // SIDEBARPLAYLIST_H
