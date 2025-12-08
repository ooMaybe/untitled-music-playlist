#include "sidebarplaylist.h"
#include "ui_sidebarplaylist.h"

sideBarPlaylist::sideBarPlaylist(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::sideBarPlaylist)
{
    ui->setupUi(this);
    setMinimumHeight(60);
}


sideBarPlaylist::~sideBarPlaylist()
{
    delete ui;
}

void sideBarPlaylist::setName(const QString &titleName)
{
    ui->labelName->setText(titleName);
}

void sideBarPlaylist::setIcon(const QPixmap &pixmap)
{
    ui->labelIcon->setPixmap(pixmap);
    ui->labelIcon->setScaledContents(true);
}
