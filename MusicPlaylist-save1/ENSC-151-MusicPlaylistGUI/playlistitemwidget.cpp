#include "playlistitemwidget.h"
#include "ui_playlistitemwidget.h"

PlaylistItemWidget::PlaylistItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlaylistItemWidget)
{
    ui->setupUi(this);
    setMinimumHeight(60);
}

PlaylistItemWidget::~PlaylistItemWidget()
{
    delete ui;
}

void PlaylistItemWidget::setName(const QString &titleName)
{
    ui->labelName->setText(titleName);
}

void PlaylistItemWidget::setArtist(const QString &titleArtist)
{
    ui->labelArtist->setText(titleArtist);
}

void PlaylistItemWidget::setDuration(const QString &titleDuration)
{
    ui->labelDuration->setText(titleDuration);
}

void PlaylistItemWidget::setDate(const QString &titleDate)
{
    ui->labelDate->setText(titleDate);
}

void PlaylistItemWidget::setIcon(const QPixmap &pixmap)
{
    ui->labelIcon->setPixmap(pixmap);
    ui->labelIcon->setScaledContents(true); // so it fits nicely
}
