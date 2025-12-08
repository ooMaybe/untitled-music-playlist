#ifndef PLAYLISTPAGE_H
#define PLAYLISTPAGE_H

#include <QWidget>

namespace Ui {
class playlistpage;
}

class playlistpage : public QWidget
{
    Q_OBJECT

public:
    explicit playlistpage(QWidget *parent = nullptr);
    ~playlistpage();

private:
    Ui::playlistpage *ui;
};

#endif 
