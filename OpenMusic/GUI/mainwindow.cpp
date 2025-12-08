#include <QMessageBox>
#include <QTimer>
#include <QFileIconProvider>
#include <QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistitemwidget.h"
#include "playlistlistwidgetyes.h"

MainWindow::MainWindow(YTDLPManager &manager, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), ytdlpManager(manager)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    // Tree widget for the other one (songsDownload)
    ui->songsDownload->setColumnCount(4);
    ui->songsDownload->setHeaderLabels(QStringList() << "Title" << "Artist" << "Duration" << "Date");
    ui->songsDownload->setColumnWidth(0, 250);
    ui->songsDownload->setIconSize(QSize(64, 64));

    ui->searchList->setColumnCount(4);
    ui->searchList->setHeaderLabels(QStringList() << "Title" << "Artist" << "Duration" << "Date");
    ui->searchList->setColumnWidth(0, 250);
    ui->searchList->setIconSize(QSize(64, 64));

    ui->searchList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->searchList, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_searchList_customContextMenuRequested);

    ui->sideBarPlaylist->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->sideBarPlaylist, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_sideBarPlaylist_customContextMenuRequested);

    // Connect media player signals to UI updates
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::positionChanged,
            this, [this](qint64 position) {
                updateProgress(static_cast<int>(position));
            });
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::durationChanged,
            this, [this](qint64 duration) {
                updateDuration(static_cast<int>(duration));
            });
    connect(ui->homeButton, &QPushButton::clicked,
            this, &MainWindow::on_homeButton_clicked);


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

    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Norbu", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap(":/icons/music.png"));
    addSidePlaylist("Alex", QPixmap("C:\\Users\\phomm\\Downloads\\7fc832400bfc5bad5ed10ee7a9b802f3"));
    addSidePlaylist("Alex", QPixmap("C:\\Users\\phomm\\Downloads\\7fc832400bfc5bad5ed10ee7a9b802f3"));
    addSidePlaylist("Karam", QPixmap("C:\\Users\\phomm\\Downloads\\7fc832400bfc5bad5ed10ee7a9b802f3"));

    addToPlaylistPage("Sorry", "Dany", "4:20", "January 6, 2020", QPixmap("C:\\Users\\phomm\\Downloads\\7fc832400bfc5bad5ed10ee7a9b802f3"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addToPlaylistPage(const QString &titleName,
                                   const QString &titleArtist,
                                   const QString &titleDuration,
                                   const QString &titleDate,
                                   const QPixmap &icon)
{
    auto *tree = ui->mainPlaylistTree;
    tree->setColumnCount(5);
    tree->setHeaderLabels({"Icon", "Name", "Artist", "Duration", "Date Added"});
    tree->setIconSize(QSize(64, 64));
    tree->setColumnWidth(0, 80);
    tree->setUniformRowHeights(false);

    QTreeWidgetItem *item = new QTreeWidgetItem(tree);
    tree->addTopLevelItem(item);

    auto *iconLabel = new QLabel(tree);
    iconLabel->setPixmap(icon.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setAlignment(Qt::AlignCenter);
    tree->setItemWidget(item, 0, iconLabel);

    item->setText(1, titleName);
    item->setText(2, titleArtist);
    item->setText(3, titleDuration);
    item->setText(4, titleDate);

    tree->setStyleSheet(
        "QTreeWidget::item { padding-left: 10px; }"
        );
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

    // We try to load thumbnail here please
    loadThumbnailForTreeItem(thumbnail, item);
}
}
void MainWindow::addSidePlaylist(const QString &titleName,
                                 const QPixmap &icon)
{
    auto *tree = ui->sideBarPlaylist;
    tree->setColumnCount(2);
    tree->setHeaderLabels({"Icon", "Name"});
    tree->setIconSize(QSize(64, 64));
    tree->setColumnWidth(0, 80);
    tree->setUniformRowHeights(false);

    QTreeWidgetItem *item = new QTreeWidgetItem(tree);
    tree->addTopLevelItem(item);

    auto *iconLabel = new QLabel(tree);
    iconLabel->setPixmap(icon.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    iconLabel->setAlignment(Qt::AlignCenter);
    tree->setItemWidget(item, 0, iconLabel);

    item->setText(1, titleName);

    tree->setStyleSheet(
        "QTreeWidget::item { padding-left: 10px; }"
        );
}


void MainWindow::addSong(const QString &titleName,
                         const QString &titleArtist,
                         const QString &titleDuration,
                         const QString &titleDate,
                         const QPixmap &icon,
                         const QString &filePath)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->songsDownload);

    item->setText(0, titleName);
    item->setText(1, titleArtist);
    item->setText(2, titleDuration);
    item->setText(3, titleDate);
    item->setIcon(0, QIcon(icon));

    // Store file path for playing later
    item->setData(0, Qt::UserRole, filePath);

    ui->songsDownload->addTopLevelItem(item);
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

void MainWindow::on_sideBarPlaylist_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->sideBarPlaylist->itemAt(pos);
    if (!item) return;

    QMenu menu(this);

    QAction *openAction = menu.addAction("Open");
    QAction *removeAction = menu.addAction("Remove");

    QAction *chosen = menu.exec(ui->sideBarPlaylist->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    ui->stackedWidget->setCurrentIndex(1);

    if (chosen == openAction) {

        QWidget *widget = ui->sideBarPlaylist->itemWidget(item, 0);
        auto *label = qobject_cast<QLabel*>(widget);

        QPixmap pixmap = label->pixmap();

        ui->labelPlaylistImageMain->setPixmap(
            pixmap.scaled(ui->labelPlaylistImageMain->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );

        ui->mainPlaylistName->setText(item->text(1));

    } else if (chosen == removeAction) {
        delete item;
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
        ytdlpManager.playSong(url.toStdString());
        ui->songNameBox->setText(item->text(0));

        // Try to please load thumbnail :D
        loadThumbnail(thumbnail, ui->songImage);

        ui->stopButton->setText("Stop");
    } else if (chosen == downloadAction) {
        qDebug() << "Download:" << item->text(0);
        qDebug() << "URL:" << url;

        bool success = ytdlpManager.downloadSong(url.toStdString(), item->text(0).toStdString());

        if (success) {
            QMessageBox::information(this, "Success", "Download complete!\n\nSaved to Downloads folder.");

            addSong(item->text(0));

        } else {
            QMessageBox::critical(this, "Error", "Download failed!");
        }
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
    // TODO: Make it pause/play instead of STOP)
    ytdlpManager.stopSong();

    // There are better ways but this is easiest.
    if (ui->stopButton->text().contains("Play")){
        ui->stopButton->setText("Stop");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
    }else{
        ui->stopButton->setText("Play");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-start"));
    }
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

// NOTE: this does NOT support webpg
void MainWindow::loadThumbnailForTreeItem(const QString &url, QTreeWidgetItem *item) {
    // Check if URL is valid
    if (url.isEmpty() || !url.startsWith("http")) {
        qDebug() << "[MainWindow] Invalid thumbnail URL:" << url;
        return;
    }

    QNetworkRequest request(url);
    // Add user agent to avoid being blocked
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    QNetworkReply *reply = networkManager->get(request);

    // Set timeout (2 seconds)
    QTimer::singleShot(2000, reply, [reply]() {
        if (reply->isRunning()) {
            qDebug() << "[MainWindow] Thumbnail request timed out";
            reply->abort();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [reply, item, url]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);

            if (!pixmap.isNull()) {
                // Scale to a reasonable size for tree view (e.g., 64x64)
                QPixmap scaledPixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                item->setIcon(0, QIcon(scaledPixmap));
            } else {
                qDebug() << "[MainWindow] Failed to decode image from:" << url;
            }
        } else {
            qDebug() << "[MainWindow] Failed to load thumbnail:" << reply->errorString() << "URL:" << url;
        }
        reply->deleteLater();
    });
}



void MainWindow::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    qDebug() << "Hi";
}

