#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistitemwidget.h"
#include "playlistlistwidgetyes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SongHomeList->setStyleSheet("QListWidget::item { border: none; padding: 0; margin: 0; }");
    ui->SongHomeList->setUniformItemSizes(false);
    ui->SongHomeList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->SongHomeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->SongHomeList->setFocusPolicy(Qt::NoFocus);
    ui->SongHomeList->setSpacing(0);


    // Example: add one song
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Nightcall", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));
    addSong("Poop", "Kavinsky", "3:32", "Augest 3, 2025", QPixmap(":/icons/music.png"));

    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Canada", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Pee", "23", QPixmap(":/icons/music.png"));
    addPlaylist("Poop", "23", QPixmap(":/icons/music.png"));

    addSearch("Hello", "David", "3:20", "Augest 13, 2007", QPixmap(":/icons/music.png"));
    addSearch("Hello", "David", "3:20", "Augest 13, 2007", QPixmap(":/icons/music.png"));
    addSearch("Hello", "David", "3:20", "Augest 13, 2007", QPixmap(":/icons/music.png"));
    addSearch("Hello", "David", "3:20", "Augest 13, 2007", QPixmap(":/icons/music.png"));
    addSearch("Hello", "David", "3:20", "Augest 13, 2007",  QPixmap(":/icons/music.png"));
    addSearch("Goodbye", "David", "3:20", "Augest 13, 2007", QPixmap(":/icons/music.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::addSearch(const QString &titleName,
                         const QString &titleArtist,
                         const QString &titleDuration,
                         const QString &titleDate,
                         const QPixmap &icon)
{
    // 1. Create blank list item
    auto *item = new QListWidgetItem(ui->searchList);

    // 2. Create your custom widget
    auto *widget = new PlaylistItemWidget(ui->searchList);
    widget->setName(titleName);
    widget->setArtist(titleArtist);
    widget->setDuration(titleDuration);
    widget->setDate(titleDate);
    widget->setIcon(icon);

    // 3. Size the row to fit your widget
    item->setSizeHint(widget->sizeHint());

    // 4. Attach the widget to the item
    ui->searchList->setItemWidget(item, widget);
}

void MainWindow::addSong(const QString &titleName,
                         const QString &titleArtist,
                         const QString &titleDuration,
                         const QString &titleDate,
                         const QPixmap &icon)
{
    // 1. Create blank list item
    auto *item = new QListWidgetItem(ui->SongHomeList);

    // 2. Create your custom widget
    auto *widget = new PlaylistItemWidget(ui->SongHomeList);
    widget->setName(titleName);
    widget->setArtist(titleArtist);
    widget->setDuration(titleDuration);
    widget->setDate(titleDate);
    widget->setIcon(icon);

    // 3. Size the row to fit your widget
    item->setSizeHint(widget->sizeHint());

    // 4. Attach the widget to the item
    ui->SongHomeList->setItemWidget(item, widget);
}

void MainWindow::addPlaylist(const QString &titleName,
                             const QString &titleNumSong,
                             const QPixmap &icon)
{
    // 1. Create blank list item
    auto *item = new QListWidgetItem(ui->PlaylistHomeList);

    // 2. Create your custom widget
    auto *widget = new PlaylistListWidgetYes(ui->PlaylistHomeList);
    widget->setNamePlaylist(titleName);
    widget->setNumSong(titleNumSong);
    widget->setIconPlaylist(icon);

    // 3. Size the row to fit your widget
    item->setSizeHint(widget->sizeHint());

    // 4. Attach the widget to the item
    ui->PlaylistHomeList->setItemWidget(item, widget);
}
