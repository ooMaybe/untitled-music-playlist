#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistitemwidget.h"
#include "playlistlistwidgetyes.h"
#include "sidebarplaylist.h"

MainWindow::MainWindow(YTDLPManager &manager, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), ytdlpManager(manager)
{
    ui->setupUi(this);

    // Initialize network manager for loading images
    networkManager = new QNetworkAccessManager(this);

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

    // Connect media player signals to UI updates
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::positionChanged,
            this, [this](qint64 position) {
                updateProgress(static_cast<int>(position));
            });
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::durationChanged,
            this, [this](qint64 duration) {
                updateDuration(static_cast<int>(duration));
            });
    ui->sidePlaylistList->setResizeMode(QListView::Adjust);
    ui->sidePlaylistList->setUniformItemSizes(false);
    ui->sidePlaylistList->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    ui->sidePlaylistList->setStyleSheet("QListWidget::item { padding: 0; margin: 0; }");

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

    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Norbu", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Karam", QPixmap(":/icons/music.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addSearch(const QString &titleName,
                           const QString &titleArtist,
                           const QString &titleDuration,
                           const QString &titleDate,
                           const QPixmap &icon,
                           const QString &url,
                           const QString &thumbnail)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->searchList);

    item->setText(0, titleName);
    item->setText(1, titleArtist);
    item->setText(2, titleDuration);
    item->setText(3, titleDate);
    item->setIcon(0, icon);

    // Store URL and thumbnail
    item->setData(0, Qt::UserRole, url);
    item->setData(0, Qt::UserRole + 1, thumbnail);

    ui->searchList->addTopLevelItem(item);
void MainWindow::addSidePlaylist(const QString &titleName,
                                 const QPixmap &icon)
{
    // 1. Create blank list item
    auto *item = new QListWidgetItem(ui->sidePlaylistList);

    // 2. Create your custom widget
    auto *widget = new sideBarPlaylist(ui->sidePlaylistList);
    widget->setName(titleName);
    widget->setIcon(icon);

    // 3. Size the row to fit your widget
    item->setSizeHint(widget->sizeHint());

    // 4. Attach the widget to the item
    ui->sidePlaylistList->setItemWidget(item, widget);
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


void MainWindow::on_searchButton_clicked()
{
    std::string query = ui->searchBar->text().toStdString();
    if (query.length() == 0){
        QMessageBox::critical(this, "Error", "You must typed something into the search bar before searching!");
        return;
    }

    std::vector<SongResult> songs = ytdlpManager.searchSongs(query, 10);

    ui->searchList->clear();
    ui->searchBar->clear();

    for (const auto& s : songs) {
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
                  QPixmap(":/icons/music.png"),
                  QString::fromStdString(s.url),
                  QString::fromStdString(s.thumbnail));
    }
}

void MainWindow::on_searchList_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->searchList->itemAt(pos);
    if (!item) return;

    QMenu menu(this);

    QAction *playAction = menu.addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Play");
    QAction *downloadAction = menu.addAction(style()->standardIcon(QStyle::SP_ArrowDown), "Download");

    QAction *chosen = menu.exec(ui->searchList->viewport()->mapToGlobal(pos));
    if (!chosen)
        return;

    // Retrieve the stored URL
    QString url = item->data(0, Qt::UserRole).toString();
    QString thumbnail = item->data(0, Qt::UserRole + 1).toString();

    if (chosen == playAction) {
        qDebug() << "Play:" << item->text(0);
        qDebug() << "URL:" << url;

        // NOTE: This works but it only plays a "preview". Youtube drops the URL after 30-60 seconds so it might stop working.
        ytdlpManager.playSong(url.toStdString());  // USE THE URL
        ui->songNameBox->setText(item->text(0));

        // Try to please load thumbnail :D
        loadThumbnail(thumbnail, ui->songImage);
    } else if (chosen == downloadAction) {
        qDebug() << "Download:" << item->text(0);
        qDebug() << "URL:" << url;
        //ytdlpManager.downloadSong(url.toStdString());  // USE THE URL
    }
}

void MainWindow::updateProgress(int position) {
    // Update progress bar
    if (ui->songProgress->maximum() > 0) {
        ui->songProgress->setValue(position);
    }

    // Update starting time (current position)
    int seconds = (position / 1000) % 60;
    int minutes = (position / 1000 / 60) % 60;
    int hours = (position / 1000 / 60 / 60);

    QString timeStr;
    if (hours > 0) {
        timeStr = QString("%1:%2:%3")
        .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    } else {
        timeStr = QString("%1:%2")
        .arg(minutes)
            .arg(seconds, 2, 10, QChar('0'));
    }

    ui->startingBox->setText(timeStr);
}

void MainWindow::updateDuration(int duration) {
    // Set progress bar range
    ui->songProgress->setRange(0, duration);

    // Update duration time (total length)
    int seconds = (duration / 1000) % 60;
    int minutes = (duration / 1000 / 60) % 60;
    int hours = (duration / 1000 / 60 / 60);

    QString timeStr;
    if (hours > 0) {
        timeStr = QString("%1:%2:%3")
        .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    } else {
        timeStr = QString("%1:%2")
        .arg(minutes)
            .arg(seconds, 2, 10, QChar('0'));
    }

    // Percentage displayed.
    ui->durationBox->setText(timeStr);
}

void MainWindow::on_stopButton_clicked()
{
    // TODO: Make it pause/play instead of STOP
    ytdlpManager.stopSong();
}

void MainWindow::loadThumbnail(const QString &url, QLabel *label) {
    QNetworkRequest request(url);
    QNetworkReply *reply = networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [reply, label]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);

            if (!pixmap.isNull()) {
                label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        } else {
            qDebug() << "[MainWindow] Failed to load thumbnail:" << reply->errorString();
            label->setPixmap(QPixmap(":/icons/music.png"));  // Fallback
        }
        reply->deleteLater();
    });
}
