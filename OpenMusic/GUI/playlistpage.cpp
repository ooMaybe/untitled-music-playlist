#include "playlistpage.h"
#include "ui_playlistpage.h"

playlistpage::playlistpage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::playlistpage)
{
    ui->setupUi(this);
}

playlistpage::~playlistpage()
{
    delete ui;
}
