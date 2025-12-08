#include <QMessageBox>
#include <QTimer>
#include <QFileIconProvider>
#include <QDir>
#include <QLabel>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playlistitemwidget.h"
#include "playlistlistwidgetyes.h"

QList<QString> playlistNames;

MainWindow::MainWindow(YTDLPManager &manager, Backend &backendRef, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), ytdlpManager(manager), backend(backendRef), isPlayingFromFile(false), currentSongIndex(-1)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    // Tree widget for the other one (songsDownload)
    ui->songsDownload->setColumnCount(4);
    ui->songsDownload->setHeaderLabels(QStringList() << "Title" << "Artist" << "Duration" << "Date");
    ui->songsDownload->setColumnWidth(0, 250);
    ui->songsDownload->setIconSize(QSize(64, 64));

    // Tree WIdget for song results
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

    // Context Menu for songsDownload
    ui->songsDownload->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->songsDownload, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_songsDownload_customContextMenuRequested);
    
    // Context menu for mainPlaylistTree
    ui->mainPlaylistTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->mainPlaylistTree, &QWidget::customContextMenuRequested,
            this, &MainWindow::on_mainPlaylistTree_customContextMenuRequested);

    // Connect media player signals to UI updates
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::positionChanged,
            this, [this](qint64 position) {
                updateProgress(static_cast<int>(position));
            });
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::durationChanged,
            this, [this](qint64 duration) {
                updateDuration(static_cast<int>(duration));
            });
    
    // Connect media status to handle auto-play next song
    connect(ytdlpManager.getMediaPlayer(), &QMediaPlayer::mediaStatusChanged,
            this, &MainWindow::onMediaStatusChanged);
    
    // Load playlists and downloaded songs from database
    loadData();
}

MainWindow::~MainWindow()
{
    delete ui;
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
        currentPlaylist = item->text(1);  // Store current playlist name
        
        QWidget *widget = ui->sideBarPlaylist->itemWidget(item, 0);
        auto *label = qobject_cast<QLabel*>(widget);

        QPixmap pixmap = label->pixmap();

        ui->labelPlaylistImageMain->setPixmap(
            pixmap.scaled(ui->labelPlaylistImageMain->size(),
                          Qt::KeepAspectRatio,
                          Qt::SmoothTransformation)
            );

        ui->mainPlaylistName->setText(currentPlaylist);
        
        // Load songs from database
        ui->mainPlaylistTree->clear();
        std::vector<PlaylistSong> songs = backend.getPlaylistSongs(currentPlaylist.toStdString());
        
        for (const auto& song : songs) {
            QPixmap thumbnail;
            if (!song.thumbnailPath.empty() && QFile::exists(QString::fromStdString(song.thumbnailPath))) {
                thumbnail.load(QString::fromStdString(song.thumbnailPath));
            } else {
                thumbnail = QPixmap(":/icons/music.png");
            }
            
            QTreeWidgetItem *songItem = new QTreeWidgetItem(ui->mainPlaylistTree);
            
            auto *iconLabel = new QLabel(ui->mainPlaylistTree);
            iconLabel->setPixmap(thumbnail.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            iconLabel->setAlignment(Qt::AlignCenter);
            ui->mainPlaylistTree->setItemWidget(songItem, 0, iconLabel);
            
            songItem->setText(1, QString::fromStdString(song.title));
            songItem->setText(2, QString::fromStdString(song.artist));
            songItem->setText(3, QString::fromStdString(song.duration));
            songItem->setText(4, QString::fromStdString(song.dateAdded));
            
            // Store song ID and file path for playing/deleting
            songItem->setData(0, Qt::UserRole, song.id);
            songItem->setData(0, Qt::UserRole + 1, QString::fromStdString(song.filePath));
            
            ui->mainPlaylistTree->addTopLevelItem(songItem);
        }
        
    } else if (chosen == removeAction) {
        QString playlistName = item->text(1);
        auto reply = QMessageBox::question(this, "Delete Playlist",
                                           "Are you sure you want to delete playlist '" + playlistName + "'?",
                                           QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            backend.deletePlaylist(playlistName.toStdString());
            playlistNames.removeOne(playlistName);
            delete item;
            QMessageBox::information(this, "Deleted", "Playlist deleted successfully.");
        }
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
        
        isPlayingFromFile = false;  // Playing from web URL
        currentSongIndex = -1;  // Not playing from playlist, disable auto-play

        // Try to please load thumbnail :D
        loadThumbnail(thumbnail, ui->songImage);

        ui->stopButton->setText("Stop");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
    } else if (chosen == downloadAction) {
        qDebug() << "Download:" << item->text(0);
        qDebug() << "URL:" << url;

        bool success = ytdlpManager.downloadSong(url.toStdString(), item->text(0).toStdString());

        if (success) {
            QMessageBox::information(this, "Success", "Download complete!\n\nSaved to Downloads folder.");

            // Get paths
            QString exeDir = QCoreApplication::applicationDirPath();
            QString downloadFolder = QDir(exeDir).filePath("data/Downloads");

            // Sanitize title (same as in YTDLPManager)
            QString safeTitle = item->text(0);
            safeTitle.replace(QRegularExpression("[<>:\"/\\\\|?*]"), "");

            QString mp3Path = QDir(downloadFolder).filePath(safeTitle + ".mp3");
            QString pngPath = QDir(downloadFolder).filePath(safeTitle + ".png");

            // Load thumbnail
            QPixmap thumbnail;
            if (QFile::exists(pngPath)) {
                thumbnail.load(pngPath);
                thumbnail = thumbnail.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            } else {
                thumbnail = QPixmap(":/icons/music.png");
            }
            
            QString dateAdded = QDateTime::currentDateTime().toString("MMM d, yyyy");
            
            // Save to database
            backend.saveDownloadedSong(item->text(0).toStdString(),  // Title
                                      item->text(1).toStdString(),  // Artist
                                      item->text(2).toStdString(),  // Duration
                                      dateAdded.toStdString(),      // Date
                                      mp3Path.toStdString(),        // File path
                                      pngPath.toStdString());       // Thumbnail path

            // Add to songsDownload tree widget
            addSong(item->text(0),           // Title
                    item->text(1),           // Artist
                    item->text(2),           // Duration
                    dateAdded,               // Date
                    thumbnail,               // Thumbnail
                    mp3Path);                // File path

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

void MainWindow::loadThumbnailForTreeItem(const QString &url, QTreeWidgetItem *item) {
    // Check if URL is valid
    if (url.isEmpty() || !url.startsWith("http")) {
        qDebug() << "[MainWindow] Invalid thumbnail URL:" << url;
        return;
    }

    QString fixedUrl = url;

    // 1. Replace the WEBP path component 'vi_webp' with the standard 'vi'
    // Example: https://i.ytimg.com/vi_webp/ -> https://i.ytimg.com/vi/
    fixedUrl.replace("/vi_webp/", "/vi/", Qt::CaseInsensitive);

    // 2. Replace the file extension '.webp' with '.jpg'
    // Example: maxresdefault.webp -> maxresdefault.jpg
    fixedUrl.replace(".webp", ".jpg", Qt::CaseInsensitive);
    // -------------------------------------------------------------------------------

    QNetworkRequest request(fixedUrl); // Use the guaranteed JPEG URL
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36");

    QNetworkReply *reply = networkManager->get(request);

    // Set timeout (2 seconds)
    QTimer::singleShot(2000, reply, [reply]() {
        if (reply->isRunning()) {
            qDebug() << "[MainWindow] Thumbnail request timed out";
            reply->abort();
        }
    });

    connect(reply, &QNetworkReply::finished, this, [reply, item, fixedUrl]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray imageData = reply->readAll();

            // Use QPixmap::loadFromData directly, as the data should now be JPEG
            QPixmap pixmap;
            pixmap.loadFromData(imageData);

            if (!pixmap.isNull()) {
                // Scale and set the icon
                QPixmap scaledPixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                item->setIcon(0, QIcon(scaledPixmap));
            } else {
                // If JPEG fails (e.g., image not found on server), use a fallback icon
                qDebug() << "[MainWindow] Failed to load/decode JPEG data from:" << fixedUrl;
                item->setIcon(0, QIcon(":/icons/music.png")); // Fallback
            }
        } else {
            qDebug() << "[MainWindow] Network error loading thumbnail:" << reply->errorString() << "URL:" << fixedUrl;
        }
        reply->deleteLater();
    });
}

void MainWindow::on_songsDownload_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->songsDownload->itemAt(pos);
    if (!item) return;

    QMenu menu(this);

    QAction *playAction = menu.addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Play");
    QAction *deleteAction = menu.addAction(style()->standardIcon(QStyle::SP_TrashIcon), "Delete");
    QMenu *addToMenu = menu.addMenu(style()->standardIcon(QStyle::SP_ComputerIcon), "Add to Playlist");

    QList<QAction*> playlistActions;
    for (const QString &name : playlistNames) {
        playlistActions.append(addToMenu->addAction("Add to " + name));
    }

    QAction *chosen = menu.exec(ui->songsDownload->viewport()->mapToGlobal(pos));
    if (!chosen)
        return;

    QString filePath = item->data(0, Qt::UserRole).toString();

    if (chosen == playAction) {
        qDebug() << "[MainWindow] Playing local file:" << filePath;

        if (QFile::exists(filePath)) {
            // Play from LOCAL FILE (not URL!)
            ytdlpManager.getMediaPlayer()->setSource(QUrl::fromLocalFile(filePath));
            ytdlpManager.getMediaPlayer()->play();
            
            isPlayingFromFile = true;  // Playing from local file
            currentSongIndex = -1;  // Not playing from playlist, disable auto-play

            ui->songNameBox->setText(item->text(0));

            // Load thumbnail from local PNG
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");

            if (QFile::exists(pngPath)) {
                QPixmap pixmap(pngPath);
                ui->songImage->setPixmap(pixmap.scaled(ui->songImage->size(),
                                                       Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation));
            } else {
                ui->songImage->setPixmap(QPixmap(":/icons/music.png"));
            }

            ui->stopButton->setText("Stop");
            ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        } else {
            QMessageBox::warning(this, "Error", "File not found!\n\n" + filePath);
        }

    } else if (chosen == deleteAction) {
        auto reply = QMessageBox::question(this, "Delete Song",
                                           "Are you sure you want to delete:\n" + item->text(0),
                                           QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            ytdlpManager.getMediaPlayer()->setSource(QUrl());

            // Delete from database
            backend.removeDownloadedSong(filePath.toStdString());

            // Delete MP3 file
            if (QFile::exists(filePath)) {
                QFile::remove(filePath);
            }

            // Delete PNG thumbnail
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");
            if (QFile::exists(pngPath)) {
                QFile::remove(pngPath);
            }

            // Remove from tree
            delete item;

            qDebug() << "[MainWindow] Deleted:" << filePath;
        }
    }

    for (int i = 0; i < playlistActions.size(); i++) {
        if (chosen == playlistActions[i]) {

            QString playlistName = playlistNames[i];
            QString title = item->text(0);
            QString artist = item->text(1);
            QString duration = item->text(2);
            QString date = item->text(3);
            QString filePath = item->data(0, Qt::UserRole).toString();
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");  // Create separate copy for thumbnail

            qDebug() << "[MainWindow] Adding to playlist:";
            qDebug() << "  Title:" << title;
            qDebug() << "  FilePath:" << filePath;
            qDebug() << "  ThumbnailPath:" << pngPath;

            // Save to database
            backend.addSongToPlaylist(playlistName.toStdString(),
                                     title.toStdString(),
                                     artist.toStdString(),
                                     duration.toStdString(),
                                     date.toStdString(),
                                     filePath.toStdString(),
                                     pngPath.toStdString());

            QMessageBox::information(this,
                                     "Added",
                                     "Added \"" + title + "\" to playlist \"" + playlistName + "\".");

            return;
        }
    }
}

void MainWindow::on_homeButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    qDebug() << "Hi";
}

void MainWindow::on_mainPlaylistTree_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = ui->mainPlaylistTree->itemAt(pos);
    if (!item || currentPlaylist.isEmpty()) return;

    QMenu menu(this);

    QAction *playAction = menu.addAction(style()->standardIcon(QStyle::SP_MediaPlay), "Play");
    QAction *deleteAction = menu.addAction(style()->standardIcon(QStyle::SP_TrashIcon), "Remove from Playlist");

    QAction *chosen = menu.exec(ui->mainPlaylistTree->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    if (chosen == playAction) {
        QString filePath = item->data(0, Qt::UserRole + 1).toString();
        
        qDebug() << "[MainWindow] Attempting to play from playlist:";
        qDebug() << "  Song:" << item->text(1);
        qDebug() << "  FilePath:" << filePath;
        qDebug() << "  File exists:" << QFile::exists(filePath);
        
        if (QFile::exists(filePath)) {
            QUrl fileUrl = QUrl::fromLocalFile(filePath);
            qDebug() << "  Setting source:" << fileUrl;
            
            ytdlpManager.getMediaPlayer()->setSource(fileUrl);
            ytdlpManager.getMediaPlayer()->play();
            
            isPlayingFromFile = true;  // Playing from local file
            currentSongIndex = ui->mainPlaylistTree->indexOfTopLevelItem(item);  // Track position for auto-play

            ui->songNameBox->setText(item->text(1));

            // Load thumbnail
            QString pngPath = filePath;
            pngPath.replace(".mp3", ".png");

            if (QFile::exists(pngPath)) {
                QPixmap pixmap(pngPath);
                ui->songImage->setPixmap(pixmap.scaled(ui->songImage->size(),
                                                       Qt::KeepAspectRatio,
                                                       Qt::SmoothTransformation));
            } else {
                ui->songImage->setPixmap(QPixmap(":/icons/music.png"));
            }

            ui->stopButton->setText("Stop");
            ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
        } else {
            QMessageBox::warning(this, "Error", "File not found!\n\n" + filePath);
        }

    } else if (chosen == deleteAction) {
        int songId = item->data(0, Qt::UserRole).toInt();
        
        auto reply = QMessageBox::question(this, "Remove Song",
                                           "Remove \"" + item->text(1) + "\" from this playlist?",
                                           QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::Yes) {
            // Remove from database
            backend.removeSongFromPlaylist(currentPlaylist.toStdString(), songId);
            
            // Remove from UI
            delete item;
            
            QMessageBox::information(this, "Removed", "Song removed from playlist.");
        }
    }
}

void MainWindow::on_chooseImageButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open PNG Image"),
        QDir::homePath(), tr("PNG Images (*.png)"));

    ui->chooseImageButton->setText(filePath);
}


void MainWindow::on_createPlaylistButton_clicked()
{
    if (ui->playlistBox->text().length() == 0){
        QMessageBox::critical(this, "Error", "You must give the playlist a name!");
        return;
    }

    if (ui->chooseImageButton->text().length() == 0){
        QMessageBox::critical(this, "Error", "You must choose a valid image!");
        return;
    }

    QString name = ui->playlistBox->text();
    QString imagePath = ui->chooseImageButton->text();

    addSidePlaylist(name, imagePath);
    playlistNames.append(name);
    
    // Save to database
    backend.savePlaylist(name.toStdString(), imagePath.toStdString());
    
    QMessageBox::information(this, "Success", "Playlist '" + name + "' created!");
    
    ui->playlistBox->clear();
    ui->chooseImageButton->setText("");
}

void MainWindow::on_forwardTen_clicked()
{
    if (!isPlayingFromFile) {
        QMessageBox::information(this, "Feature Not Available", 
                                "Skip forward/backward is only available when playing from downloaded files.\n\n"
                                "Web streaming does not support this feature.");
        return;
    }
    
    QMediaPlayer *player = ytdlpManager.getMediaPlayer();
    qint64 currentPos = player->position();
    qint64 newPos = currentPos + 10000;  // Add 10 seconds (10000 milliseconds)
    
    // Don't go past the end
    if (newPos < player->duration()) {
        player->setPosition(newPos);
        qDebug() << "[MainWindow] Skipped forward 10 seconds";
    } else {
        player->setPosition(player->duration());
        qDebug() << "[MainWindow] Skipped to end of track";
    }
}

void MainWindow::loadData()
{
    // Load playlists from database
    std::vector<Playlist> playlists = backend.getAllPlaylists();
    
    for (const auto& playlist : playlists) {
        QString name = QString::fromStdString(playlist.name);
        QString imagePath = QString::fromStdString(playlist.imagePath);
        
        addSidePlaylist(name, QPixmap(imagePath));
        playlistNames.append(name);
    }
    
    // Load downloaded songs from database
    std::vector<DownloadedSong> songs = backend.getAllDownloadedSongs();
    
    for (const auto& song : songs) {
        QString filePath = QString::fromStdString(song.filePath);
        
        // Check if file still exists
        if (!QFile::exists(filePath)) {
            continue;  // Skip deleted files
        }
        
        QString title = QString::fromStdString(song.title);
        QString artist = QString::fromStdString(song.artist);
        QString duration = QString::fromStdString(song.duration);
        QString dateAdded = QString::fromStdString(song.dateAdded);
        QString thumbnailPath = QString::fromStdString(song.thumbnailPath);
        
        // Load thumbnail
        QPixmap thumbnail;
        if (!thumbnailPath.isEmpty() && QFile::exists(thumbnailPath)) {
            thumbnail.load(thumbnailPath);
            thumbnail = thumbnail.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        } else {
            thumbnail = QPixmap(":/icons/music.png");
        }
        
        // Add to songsDownload tree
        addSong(title, artist, duration, dateAdded, thumbnail, filePath);
    }
    
    qDebug() << "[MainWindow] Loaded" << playlists.size() << "playlists and" << songs.size() << "songs";
}


void MainWindow::on_backwardTen_clicked()
{
    if (!isPlayingFromFile) {
        QMessageBox::information(this, "Feature Not Available", 
                                "Skip forward/backward is only available when playing from downloaded files.\n\n"
                                "Web streaming does not support this feature.");
        return;
    }
    
    QMediaPlayer *player = ytdlpManager.getMediaPlayer();
    qint64 currentPos = player->position();
    qint64 newPos = currentPos - 10000;  // Subtract 10 seconds (10000 milliseconds)
    
    // Don't go before the beginning
    if (newPos > 0) {
        player->setPosition(newPos);
        qDebug() << "[MainWindow] Skipped backward 10 seconds to";
    } else {
        player->setPosition(0);
        qDebug() << "[MainWindow] Skipped to beginning of track";
    }
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    // When song finishes, play next song in playlist if we're playing from a playlist
    if (status == QMediaPlayer::EndOfMedia && currentSongIndex >= 0 && !currentPlaylist.isEmpty()) {
        qDebug() << "[MainWindow] Song ended, playing next in playlist";
        playNextSongInPlaylist();
    }
}

void MainWindow::playNextSongInPlaylist()
{
    int totalSongs = ui->mainPlaylistTree->topLevelItemCount();
    
    if (totalSongs == 0) {
        qDebug() << "[MainWindow] No songs in playlist";
        return;
    }
    
    // If only one song, replay it
    if (totalSongs == 1) {
        qDebug() << "[MainWindow] Only one song in playlist, replaying";
        QTreeWidgetItem *item = ui->mainPlaylistTree->topLevelItem(0);
        QString filePath = item->data(0, Qt::UserRole + 1).toString();
        
        if (QFile::exists(filePath)) {
            ytdlpManager.getMediaPlayer()->setSource(QUrl::fromLocalFile(filePath));
            ytdlpManager.getMediaPlayer()->play();
            currentSongIndex = 0;
        }
        return;
    }
    
    // Move to next song, loop back to start if at end
    currentSongIndex++;
    if (currentSongIndex >= totalSongs) {
        currentSongIndex = 0;
        qDebug() << "[MainWindow] Reached end of playlist, looping to first song";
    }
    
    QTreeWidgetItem *nextItem = ui->mainPlaylistTree->topLevelItem(currentSongIndex);
    if (!nextItem) {
        qDebug() << "[MainWindow] Could not find next item";
        return;
    }
    
    QString filePath = nextItem->data(0, Qt::UserRole + 1).toString();
    qDebug() << "[MainWindow] Playing next song:" << nextItem->text(1);
    qDebug() << "  FilePath:" << filePath;
    
    if (QFile::exists(filePath)) {
        ytdlpManager.getMediaPlayer()->setSource(QUrl::fromLocalFile(filePath));
        ytdlpManager.getMediaPlayer()->play();
        
        ui->songNameBox->setText(nextItem->text(1));
        
        // Load thumbnail
        QString pngPath = filePath;
        pngPath.replace(".mp3", ".png");
        
        if (QFile::exists(pngPath)) {
            QPixmap pixmap(pngPath);
            ui->songImage->setPixmap(pixmap.scaled(ui->songImage->size(),
                                                   Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation));
        } else {
            ui->songImage->setPixmap(QPixmap(":/icons/music.png"));
        }
        
        ui->stopButton->setText("Stop");
        ui->stopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
    } else {
        QMessageBox::warning(this, "Error", "File not found!\n\n" + filePath);
    }
}

