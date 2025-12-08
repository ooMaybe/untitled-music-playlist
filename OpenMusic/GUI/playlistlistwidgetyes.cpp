#include "playlistlistwidgetyes.h"
#include "ui_playlistlistwidgetyes.h"

PlaylistListWidgetYes::PlaylistListWidgetYes(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlaylistListWidgetYes)
{
    ui->setupUi(this);
    setMinimumHeight(60);
}

PlaylistListWidgetYes::~PlaylistListWidgetYes()
{
    delete ui;
}

void PlaylistListWidgetYes::setNamePlaylist(const QString &titleName)
{
    ui->labelName->setText(titleName);
}

void PlaylistListWidgetYes::setNumSong(const QString &titleNum)
{
    ui->labelNumSong->setText(titleNum);
}

void PlaylistListWidgetYes::setIconPlaylist(const QPixmap &pixmap)
{
    ui->labelIcon->setPixmap(pixmap);
    ui->labelIcon->setScaledContents(true);
}
