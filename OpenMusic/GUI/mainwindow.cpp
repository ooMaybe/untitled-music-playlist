#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistitemwidget.h"
#include "playlistlistwidgetyes.h"

MainWindow::MainWindow(YTDLPManager &manager, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), ytdlpManager(manager)
{
    ui->setupUi(this);
    ui->SongHomeList->setStyleSheet("QListWidget::item { border: none; padding: 0; margin: 0; }");
    ui->SongHomeList->setUniformItemSizes(false);
    ui->SongHomeList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->SongHomeList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->SongHomeList->setFocusPolicy(Qt::NoFocus);
    ui->SongHomeList->setSpacing(0);

    // Tree WIdget
    ui->searchList->setColumnCount(4);
    ui->searchList->setHeaderLabels(QStringList() << "Title" << "Artist" << "Duration" << "Date");
    ui->searchList->setColumnWidth(0, 250);

    // Context Menu
    ui->searchList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->searchList, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_searchList_customContextMenuRequested);

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
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->searchList);

    item->setText(0, titleName);
    item->setText(1, titleArtist);
    item->setText(2, titleDuration);
    item->setText(3, titleDate);

    // Set icon in the first column
    item->setIcon(0, icon);

    // Add top-level item to the tree
    ui->searchList->addTopLevelItem(item);
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


void MainWindow::on_searchButton_clicked()
{
    std::string query = ui->searchBar->text().toStdString();
    std::vector<SongResult> songs = ytdlpManager.searchSongs(query, 10);

    ui->searchList->clear();
    ui->searchBar->clear();

    for (const auto& s : songs) {
        std::cout << "TEST:" << s.duration;
        int durationSec = 0;

        // Check if the duration string is valid before converting
        if (!s.duration.empty()) {
            try {
                durationSec = std::stoi(s.duration);
            } catch (const std::exception &e) {
                durationSec = 0; // fallback
            }
        }

        int minutes = durationSec / 60;
        int seconds = durationSec % 60;
        QString durationStr = QString("%1:%2")
                                  .arg(minutes)
                                  .arg(seconds, 2, 10, QChar('0'));

        addSearch(QString::fromStdString(s.title),
                  QString::fromStdString(s.uploader),
                  durationStr,
                  "Unknown date",
                  QPixmap(":/icons/music.png"));
    }
}

void MainWindow::on_searchList_customContextMenuRequested(const QPoint &pos)
{
    // Get the item at the clicked position
    QTreeWidgetItem *item = ui->searchList->itemAt(pos);
    if (!item) return;

    QMenu menu(this);
    menu.addAction("Play", [item]() {
        qDebug() << "Play:" << item->text(0);
    });
    menu.addAction("Download", [item]() {
        qDebug() << "Download:" << item->text(0);
    });

    menu.exec(ui->searchList->viewport()->mapToGlobal(pos));
}

